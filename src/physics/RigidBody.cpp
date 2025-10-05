#include "utilities/Quaternion.hpp"
#include "utilities/Vector3.hpp"
#ifdef StevEngine_PHYSICS
#include "RigidBody.hpp"
#include "physics/Colliders.hpp"
#include "physics/PhysicsSystem.hpp"
#include "physics/Layers.hpp"
#include "main/Log.hpp"
#include "main/GameObject.hpp"
#include "main/Component.hpp"

#include <math.h>

namespace StevEngine::Physics {
	//Constructor
	RigidBody::RigidBody(JPH::EMotionType motionType, LayerID layer, float mass)
	  : motionType(motionType), layer(layer), mass(mass), body(nullptr) {};

	void RigidBody::Start() {
		//Create shape
		RefreshShape();

		//Create new body
		GameObject& parent = GetParent();
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, parent.GetWorldPosition(), parent.GetWorldRotation(), motionType, layer);
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
		body = physics.GetBodyInterface().CreateBody(bodySettings);
		physics.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
		//Events
		parent.Subscribe<ColliderUpdateEvent>([this](ColliderUpdateEvent) { RefreshShape(); });
	}
	void RigidBody::Deactivate() {
		if(body) physics.GetBodyInterface().DestroyBody(body->GetID());
		body = nullptr;
		if(shape) shape->Release();
		shape = nullptr;
	}
	void RigidBody::Update(double deltaTime) {
		if(motionType != JPH::EMotionType::Static) {
			GameObject& parent = GetParent();
			parent.SetPosition(body->GetPosition(), false);
			parent.SetRotation(body->GetRotation(), false);
		}
	}
	void RigidBody::TransformUpdate(bool position, bool rotation, bool scale) {
		GameObject& parent = GetParent();
		if(position || rotation) if(body != nullptr) body->SetPositionAndRotationInternal(parent.GetWorldPosition() - shape->GetCenterOfMass(), parent.GetWorldRotation());
		if(scale) RefreshShape();
	}
	void RigidBody::RefreshShape() {
		GameObject& parent = GetParent();
		Utilities::Vector3 pos = parent.GetWorldPosition();
		Utilities::Quaternion rot = parent.GetWorldRotation();
		//Find all colliders:
		std::vector<Collider*> colliders;
		colliders = parent.GetAllComponents<Collider>();
		for (int i = 0; i < parent.GetChildCount(); i++) {
			std::vector<Collider*> cc = parent.GetChild(i).GetAllComponents<Collider>();
			colliders.insert(colliders.end(), cc.begin(), cc.end());
		}
		//Create new shape
		JPH::StaticCompoundShapeSettings shapeSettings = JPH::StaticCompoundShapeSettings();
		for(Collider* col : colliders) {
			if(col->GetShape())
				shapeSettings.AddShape((col->GetParent().GetWorldPosition() + col->GetPosition() - pos), (col->GetParent().GetWorldRotation() + col->GetRotation() - rot), col->GetShape());
		}
		//Create final shape
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid()) {
			if(shape) shape->Release(); //Release old shape
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
		if(body) physics.GetBodyInterface().DestroyBody(body->GetID());
		if(shape) shape->Release();
	}

	Utilities::Stream RigidBody::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << mass << (uint32_t)motionType << layer;
		return stream;
	}
	RigidBody::RigidBody(Utilities::Stream& stream)
	  : mass(stream.Read<double>()), motionType((JPH::EMotionType)stream.Read<uint32_t>()), layer((JPH::ObjectLayer)stream.Read<int>()), body(nullptr) {}

	/** Register RigidBody as a component type */
	bool body = CreateComponents::RegisterComponentType<RigidBody>(RIGIDBODY_TYPE);
}
#endif
