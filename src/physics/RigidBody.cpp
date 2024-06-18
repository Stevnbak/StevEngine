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
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, GameObject::GetObject(gameObject)->GetWorldPosition() - shape->GetCenterOfMass(), GameObject::GetObject(gameObject)->GetWorldRotation(), motionType, layer->id);
		bodySettings.mGravityFactor = motionProperties.GravityFactor;
		bodySettings.mLinearDamping = motionProperties.LinearDamping;
		bodySettings.mAngularDamping = motionProperties.AngularDamping;
		bodySettings.mAllowedDOFs = motionProperties.AllowedDOFs;
		if(motionProperties.MaxLinearVelocity > 0) bodySettings.mMaxLinearVelocity = motionProperties.MaxLinearVelocity;
		if(motionProperties.MaxAngularVelocity > 0) bodySettings.mMaxAngularVelocity = motionProperties.MaxAngularVelocity;
		//	Set mass
		JPH::MassProperties massProperties = bodySettings.GetMassProperties();
		massProperties.ScaleToMass(mass);
		bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
		bodySettings.mMassPropertiesOverride = massProperties;
		//	Create body from settings
		body = Engine::Instance->physics->bodyInterface->CreateBody(bodySettings);
		Engine::Instance->physics->bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
	}
	void RigidBody::Update(double deltaTime) {
		if(motionType != JPH::EMotionType::Static) {
			GameObject::GetObject(gameObject)->SetPosition(body->GetWorldSpaceBounds().GetCenter(), false);
			GameObject::GetObject(gameObject)->SetRotation(body->GetRotation(), false);
		}
	}
	void RigidBody::TransformUpdate(bool position, bool rotation, bool scale) {
		if(position || rotation) if(body != nullptr) body->SetPositionAndRotationInternal(GameObject::GetObject(gameObject)->GetWorldPosition() - shape->GetCenterOfMass(), GameObject::GetObject(gameObject)->GetWorldRotation());
		if(scale) RefreshShape();
	}
	void RigidBody::RefreshShape() {
		//Find all colliders:
		colliders.clear();
		colliders = GameObject::GetObject(gameObject)->GetAllComponents<Collider>();
		for (int i = 0; i < GameObject::GetObject(gameObject)->GetChildCount(); i++) {
			std::vector<Collider*> cc = GameObject::GetObject(GameObject::GetObject(gameObject)->GetChild(i))->GetAllComponents<Collider>();
			colliders.insert(colliders.end(), cc.begin(), cc.end());
		}
		//Create new shape
		JPH::MutableCompoundShapeSettings shapeSettings = JPH::MutableCompoundShapeSettings();
		for(Collider* col : colliders) {
			shapeSettings.AddShape((GameObject::GetObject(col->gameObject)->GetWorldPosition() + col->GetPosition()), (GameObject::GetObject(col->gameObject)->GetWorldRotation() + col->GetRotation() - GameObject::GetObject(gameObject)->GetWorldRotation()), col->shape);
		}
		//Create final shape
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid()) {
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