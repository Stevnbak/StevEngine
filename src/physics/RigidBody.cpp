#ifdef StevEngine_PHYSICS
#include "RigidBody.hpp"
#include "physics/Colliders.hpp"
#include "physics/PhysicsSystem.hpp"
#include "main/Engine.hpp"
#include "physics/Layers.hpp"
#include "main/Log.hpp"
#include "scenes/GameObject.hpp"
#include "scenes/Component.hpp"

#include <math.h>
#include <iostream>

namespace StevEngine::Physics {
	//Constructor
	RigidBody::RigidBody(JPH::EMotionType motionType, Layer* layer, float mass)
		: Component("RigidBody"), motionType(motionType), layer(layer), mass(mass), body(nullptr) {};

	void RigidBody::Start() {
		//Create shape
		RefreshShape();

		//Create new body
		GameObject* parent = GetParent();
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, parent->GetWorldPosition() - shape->GetCenterOfMass(), parent->GetWorldRotation(), motionType, layer->id);
		bodySettings.mGravityFactor = motionProperties.GravityFactor;
		bodySettings.mLinearDamping = motionProperties.LinearDamping;
		bodySettings.mAngularDamping = motionProperties.AngularDamping;
		bodySettings.mAllowedDOFs = motionProperties.AllowedDOFs;
		if(motionProperties.MaxLinearVelocity > 0) bodySettings.mMaxLinearVelocity = motionProperties.MaxLinearVelocity;
		if(motionProperties.MaxAngularVelocity > 0) bodySettings.mMaxAngularVelocity = motionProperties.MaxAngularVelocity;
		//	Set mass
		JPH::MassProperties massProperties = bodySettings.GetMassProperties();
		if(massProperties.mMass == 0) {
			//If there is no default calcualted mass and inertia (from meshes) calculate as box
			massProperties.SetMassAndInertiaOfSolidBox(shape->GetLocalBounds().GetSize(), 1000.0);
		}
		massProperties.ScaleToMass(mass);
		bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
		bodySettings.mMassPropertiesOverride = massProperties;
		//	Create body from settings
		body = physics.GetBodyInterface()->CreateBody(bodySettings);
		physics.GetBodyInterface()->AddBody(body->GetID(), JPH::EActivation::Activate);

		//Events
		parent->Subscribe<ColliderUpdateEvent>([this](ColliderUpdateEvent) { RefreshShape(); });
	}
	void RigidBody::Deactivate() {
		if(body) physics.GetBodyInterface()->DestroyBody(body->GetID());
		if(shape) shape->Release();
	}
	void RigidBody::Update(double deltaTime) {
		if(motionType != JPH::EMotionType::Static) {
			GameObject* parent = GetParent();
			parent->SetPosition(body->GetWorldSpaceBounds().GetCenter(), false);
			parent->SetRotation(body->GetRotation(), false);
		}
	}
	void RigidBody::TransformUpdate(bool position, bool rotation, bool scale) {
		GameObject* parent = GetParent();
		if(position || rotation) if(body != nullptr) body->SetPositionAndRotationInternal(parent->GetWorldPosition() - shape->GetCenterOfMass(), parent->GetWorldRotation());
		if(scale) RefreshShape();
	}
	void RigidBody::RefreshShape() {
		GameObject* parent = GetParent();
		//Find all colliders:
		colliders.clear();
		colliders = parent->GetAllComponents<Collider>();
		for (int i = 0; i < parent->GetChildCount(); i++) {
			std::vector<Collider*> cc = parent->GetChild(i)->GetAllComponents<Collider>();
			colliders.insert(colliders.end(), cc.begin(), cc.end());
		}
		//Create new shape
		JPH::StaticCompoundShapeSettings shapeSettings = JPH::StaticCompoundShapeSettings();
		for(Collider* col : colliders) {
			if(col->GetShape())
				shapeSettings.AddShape((col->GetParent()->GetWorldPosition() + col->GetPosition()), (col->GetParent()->GetWorldRotation() + col->GetRotation() - parent->GetWorldRotation()), col->GetShape());
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
			Log::Error(result.GetError().c_str(), true);
		}
	}
	void RigidBody::SetMotionProperties(MotionProperties properties) {
		this->motionProperties = properties;
		if(body != nullptr && motionType != JPH::EMotionType::Static) {
			JPH::MotionProperties* setter = body->GetMotionProperties();
			setter->SetGravityFactor(properties.GravityFactor);
			setter->SetLinearDamping(properties.LinearDamping);
			setter->SetAngularDamping(properties.AngularDamping);
			if(properties.MaxLinearVelocity > 0) setter->SetMaxLinearVelocity(properties.MaxLinearVelocity);
			if(properties.MaxAngularVelocity > 0) setter->SetMaxAngularVelocity(properties.MaxAngularVelocity);
		}
	}

	RigidBody::~RigidBody() {
		if(body) physics.GetBodyInterface()->DestroyBody(body->GetID());
		if(shape) shape->Release();
	}

	YAML::Node RigidBody::Export(YAML::Node node) const {
		node["mass"] = mass;
		node["motionType"] = (uint32_t)motionType;
		node["layer"] = layer->id;
		return node;
	}
	RigidBody::RigidBody(YAML::Node node) : Component(node), mass(node["mass"].as<double>()), motionType((JPH::EMotionType)node["motionType"].as<uint32_t>()), layer(StevEngine::Physics::Layer::GetLayerById(node["layer"].as<int>())), body(nullptr) {}
}
#endif
