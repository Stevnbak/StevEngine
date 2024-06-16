#include "RigidBody.hpp"
#include <physics/System.hpp>
#include <math.h>
#include <iostream>
#include <core/Log.hpp>
#include <core/Engine.hpp>

namespace StevEngine::Physics {
	FactoryBase* bodyfactory = GameObject::AddComponentFactory<RigidBody>(std::string("RigidBody"));
	//Constructor
	RigidBody::RigidBody(JPH::EMotionType motionType, Layer* layer, float mass) : Component("RigidBody") {
		this->motionType = motionType;
		this->layer = layer;
		this->mass = mass;
	};

	void RigidBody::Start() {
		//Get collider info
		UpdateColliders();
		//Create body
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, gameObject->absPosition() - shape->GetCenterOfMass(), gameObject->absRotation(), motionType, layer->id);
		bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
		bodySettings.mMassPropertiesOverride.mMass = mass;
		body = Engine::Instance->physics->bodyInterface->CreateBody(bodySettings);
		Engine::Instance->physics->bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
	}

	void RigidBody::Update(double deltaTime) {
		if(body->GetMotionType() != JPH::EMotionType::Static) {
			///Log::Normal(std::format("Position: {},{},{}; JoltCenterPosition: {},{},{}", this->gameObject->position.X, this->gameObject->position.Y, this->gameObject->position.Z, body->GetCenterOfMassPosition().GetX(), body->GetCenterOfMassPosition().GetY(), body->GetCenterOfMassPosition().GetZ()));
			this->gameObject->position = body->GetWorldSpaceBounds().GetCenter();
		}
	}

	void RigidBody::UpdateColliders() {
		//Find all colliders:
		std::vector<Collider*> c = gameObject->GetAllComponents<Collider>();
		for (int i = 0; i < gameObject->GetChildCount(); i++) {
			GameObject* child = gameObject->GetChild(i);
			std::vector<Collider*> cc = child->GetAllComponents<Collider>();
			c.insert(c.end(), cc.begin(), cc.end());
		}
		//Clear old info
		colliders.clear();
		delete shape;
		JPH::MutableCompoundShapeSettings shapeSettings = JPH::MutableCompoundShapeSettings();
		//Add new
		for(Collider* col : c) {
			if(std::find(colliders.begin(), colliders.end(), col) == colliders.end()) {
				colliders.push_back(col);
				shapeSettings.AddShape((col->gameObject->absPosition() + col->getPosition()), (col->gameObject->absRotation() + col->getRotation()), col->shape);
			}
		}
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid())
			shape = result.Get();
		else
			Log::Error(result.GetError().c_str());
	}

	void RigidBody::Destroy() {
		delete body;
		delete shape;
		delete this;
	}

	void RigidBody::Export(tinyxml2::XMLElement* element) {
        element->SetAttribute("motionType", (uint8)motionType);
        element->SetAttribute("layer", layer->name.c_str());
        element->SetAttribute("mass", mass);
    }
    RigidBody::RigidBody(tinyxml2::XMLElement* node) : RigidBody((JPH::EMotionType)node->IntAttribute("motionType"), Layer::GetLayerByName(node->Attribute("layer")), node->FloatAttribute("mass")) {}
}