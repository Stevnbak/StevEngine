#include "RigidBody.hpp"
#include <Physics/System.hpp>
#include <math.h>
#include <iostream>
#include <Core/Log.hpp>
#include <Core/Engine.hpp>

namespace StevEngine::Physics {
	//Constructor
	RigidBody::RigidBody(JPH::EMotionType motionType, JPH::ObjectLayer layer, float mass) {
		this->motionType = motionType;
		this->layer = layer;
		this->mass = mass;
	}

	void RigidBody::Start() {
		//Get collider info
		UpdateColliders();
		//Create body
		///delete body;
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, gameObject->position, gameObject->rotation, motionType, layer);
		bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
		bodySettings.mMassPropertiesOverride.mMass = mass;
		body = physics->bodyInterface->CreateBody(bodySettings);
		physics->bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
	}

	void RigidBody::Update(double deltaTime) {
		if(body->GetMotionType() != JPH::EMotionType::Static) {
			///Log::Normal(std::format("Position: {},{},{}", body->GetPosition().GetX(), body->GetPosition().GetY(), body->GetPosition().GetZ()));
			this->gameObject->position = body->GetPosition();
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
}