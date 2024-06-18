#include "RigidBody.hpp"
#include <physics/System.hpp>
#include <math.h>
#include <iostream>
#include <core/Log.hpp>
#include <core/Engine.hpp>

namespace StevEngine::Physics {
	FactoryBase* bodyfactory = GameObject::AddComponentFactory<RigidBody>(std::string("RigidBody"));
	//Constructor
	RigidBody::RigidBody(JPH::EMotionType motionType, Layer* layer, float mass)
		: Component("RigidBody"), motionType(motionType), layer(layer), mass(mass), body(nullptr) {};

	void RigidBody::Start() {
		//Create shape
		RefreshShape();
		//Create new body
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, gameObject->GetWorldPosition() - shape->GetCenterOfMass(), gameObject->GetWorldRotation(), motionType, layer->id);
		bodySettings.mGravityFactor = motionProperties.GravityFactor;
		bodySettings.mLinearDamping = motionProperties.LinearDamping;
		bodySettings.mAngularDamping = motionProperties.AngularDamping;
		if(motionProperties.MaxLinearVelocity > 0) bodySettings.mMaxLinearVelocity = motionProperties.MaxLinearVelocity;
		if(motionProperties.MaxAngularVelocity > 0) bodySettings.mMaxAngularVelocity = motionProperties.MaxAngularVelocity;
		bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
		bodySettings.mMassPropertiesOverride.mMass = mass;
		body = Engine::Instance->physics->bodyInterface->CreateBody(bodySettings);
		
		Engine::Instance->physics->bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
	}
	void RigidBody::Update(double deltaTime) {
		if(motionType != JPH::EMotionType::Static) {
			this->gameObject->SetPosition(body->GetWorldSpaceBounds().GetCenter(), false);
			this->gameObject->SetRotation(body->GetRotation(), false);
		}
	}
	void RigidBody::TransformUpdate(bool position, bool rotation, bool scale) {
		if(position || rotation) if(body != nullptr) body->SetPositionAndRotationInternal(gameObject->GetWorldPosition() - shape->GetCenterOfMass(), gameObject->GetWorldRotation());
		if(scale) RefreshShape();
	}
	void RigidBody::RefreshShape() {
		//Find all colliders:
		colliders.clear();
		colliders = gameObject->GetAllComponents<Collider>();
		for (int i = 0; i < gameObject->GetChildCount(); i++) {
			GameObject* child = gameObject->GetChild(i);
			std::vector<Collider*> cc = child->GetAllComponents<Collider>();
			colliders.insert(colliders.end(), cc.begin(), cc.end());
		}
		//Create new shape
		JPH::MutableCompoundShapeSettings shapeSettings = JPH::MutableCompoundShapeSettings();
		for(Collider* col : colliders) {
			shapeSettings.AddShape((col->gameObject->GetWorldPosition() + col->getPosition()), (col->gameObject->GetWorldRotation() + col->getRotation() - gameObject->GetWorldRotation()), col->shape);
		}
		//Create final shape
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid()) {
			delete shape;
			shape = result.Get();
			if(body != nullptr) {
				body->SetShapeInternal(shape, false);
			}
		}
		else {
			Log::Error(result.GetError().c_str());
		}
	}
	void RigidBody::SetMotionProperties(MotionProperties properties) {
		this->motionProperties = properties;
		if(body != nullptr && motionType != JPH::EMotionType::Static) {
			JPH::MotionProperties * setter = body->GetMotionProperties();
			setter->SetGravityFactor(properties.GravityFactor);
			setter->SetLinearDamping(properties.LinearDamping);
			setter->SetAngularDamping(properties.AngularDamping);
			if(properties.MaxLinearVelocity > 0) setter->SetMaxLinearVelocity(properties.MaxLinearVelocity);
			if(properties.MaxAngularVelocity > 0) setter->SetMaxAngularVelocity(properties.MaxAngularVelocity);
		}
	}


	void RigidBody::Destroy() {
		Engine::Instance->physics->bodyInterface->DestroyBody(body->GetID());
		delete shape;
		delete this;
	}

	void RigidBody::Export(tinyxml2::XMLElement* element) {
        element->SetAttribute("motionType", (JPH::uint8)motionType);
        element->SetAttribute("layer", layer->name.c_str());
        element->SetAttribute("mass", mass);
    }
    RigidBody::RigidBody(tinyxml2::XMLElement* node) : RigidBody((JPH::EMotionType)node->IntAttribute("motionType"), Layer::GetLayerByName(node->Attribute("layer")), node->FloatAttribute("mass")) {}
}