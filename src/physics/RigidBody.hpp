#pragma once
#ifdef StevEngine_PHYSICS
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "physics/Colliders.hpp"
#include "physics/Layers.hpp"

#include <yaml-cpp/yaml.h>
//Jolt imports
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace StevEngine::Physics {
	struct MotionProperties {
		double LinearDamping;
		double AngularDamping;
		double MaxLinearVelocity;
		double MaxAngularVelocity;
		double GravityFactor;
		JPH::EAllowedDOFs AllowedDOFs;
		MotionProperties(
			double gravityFactor = 1.0,
			double linearDamping = 0.0,
			double angularDamping = 0.0,
			JPH::EAllowedDOFs allowedDOFs = JPH::EAllowedDOFs::All,
			double maxLinearVelocity = 0.0,
			double maxAngularVelocity = 0.0
		) {
			this->LinearDamping = linearDamping;
			this->AngularDamping = angularDamping;
			this->MaxLinearVelocity = maxLinearVelocity;
			this->MaxAngularVelocity = maxAngularVelocity;
			this->GravityFactor = gravityFactor;
			this->AllowedDOFs = allowedDOFs;
		}
	};

	class RigidBody final : public Component {
		friend class Collider;
		friend class StevEngine::GameObject;
		//Information
		public:
			//Basic values
			JPH::Body* GetBody() const { return body; }
			const JPH::EMotionType motionType;
			const Layer* layer;
			const float mass;
		private:
			static const bool unique = true;
			MotionProperties motionProperties;
			std::vector<Collider*> colliders;
			JPH::Body* body;
			JPH::Ref<JPH::Shape> shape;
		//Functions
		public:
			//Basic component functions
			void Start();
			void Deactivate();
			void Update(double deltaTime);
			~RigidBody();
			//Constructor
			RigidBody(JPH::EMotionType motionType, Layer* layer, float mass = 1000);
			RigidBody(YAML::Node node);
			//Other functions
			YAML::Node Export(YAML::Node node) const;
			void SetMotionProperties(MotionProperties properties);
		private:
			void RefreshShape();
			void TransformUpdate(bool position, bool rotation, bool scale);
		//Jolt Wrappers
		public:
			bool IsActive () const { return body->IsActive(); }
			bool IsStatic () const { return body->IsStatic(); }
			bool IsKinematic () const { return body->IsKinematic(); }
			bool IsDynamic () const { return body->IsDynamic(); }
			float GetFriction () const { return body->GetFriction(); }
			void SetFriction (float inFriction) { return body->SetFriction(inFriction); }
			Utilities::Vector3 GetLinearVelocity () const { return body->GetLinearVelocity(); }
			void SetLinearVelocity (Utilities::Vector3 inLinearVelocity) { return body->SetLinearVelocity(inLinearVelocity); }
			void SetLinearVelocityClamped (Utilities::Vector3 inLinearVelocity) { return body->SetLinearVelocityClamped(inLinearVelocity); }
			Utilities::Vector3 GetAngularVelocity () const { return body->GetAngularVelocity(); }
			void SetAngularVelocity (Utilities::Vector3 inAngularVelocity) { return body->SetAngularVelocity(inAngularVelocity); }
			void SetAngularVelocityClamped (Utilities::Vector3 inAngularVelocity) { return body->SetAngularVelocityClamped(inAngularVelocity); }
			Utilities::Vector3 GetPointVelocityCOM (Utilities::Vector3 inPointRelativeToCOM) const { return body->GetPointVelocityCOM(inPointRelativeToCOM); }
			Utilities::Vector3 GetPointVelocity (Utilities::Vector3 inPoint) const { return body->GetPointVelocity(inPoint); }
			void AddForce (Utilities::Vector3 inForce) { return body->AddForce(inForce); }
			void AddForce (Utilities::Vector3 inForce, Utilities::Vector3 inPosition) { return body->AddForce(inForce, inPosition); }
			void AddTorque (Utilities::Vector3 inTorque) { return body->AddTorque(inTorque); }
			void AddImpulse (Utilities::Vector3 inImpulse) { return body->AddImpulse(inImpulse); }
			void AddImpulse (Utilities::Vector3 inImpulse, Utilities::Vector3 inPosition) { return body->AddImpulse(inImpulse, inPosition); }
			void AddAngularImpulse (Utilities::Vector3 inAngularImpulse) { return body->AddAngularImpulse(inAngularImpulse); }
			void MoveKinematic (Utilities::Vector3 inTargetPosition, Utilities::Quaternion inTargetRotation, float inDeltaTime) { return body->MoveKinematic(inTargetPosition, inTargetRotation, inDeltaTime); }
	};

	inline bool body = CreateComponents::RegisterComponentType<RigidBody>("RigidBody");
}
#endif
