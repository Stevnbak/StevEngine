#pragma once
#include <core/GameObject.hpp>
#include <core/Utilities.hpp>
#include <physics/Colliders.hpp>
#include <physics/Layers.hpp>

//Jolt imports
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace StevEngine::Physics {
	class RigidBody : public Component {
		//Information
		public:
			static const bool unique = true;
			//Basic values
			JPH::Body* GetBody() { return body; }
			const JPH::EMotionType motionType;
			const Layer* layer;
			const float mass;
		private:
			std::vector<Collider*> colliders;
			JPH::Body* body;
			JPH::Ref<JPH::Shape> shape;
		//Functions
		public:
			//Basic component functions
			void Start();
			void Update(double deltaTime);
			void Draw() {}
			void Destroy();
			//Export
			void Export(tinyxml2::XMLElement* element);
			//Constructor
			RigidBody(JPH::EMotionType motionType, Layer* layer, float mass = 1);
			RigidBody(tinyxml2::XMLElement* element);
		private:
			//Other functions
			void UpdateColliders();
			void UpdateBody();
		//Jolt Wrappers
		public:
			bool IsActive () const { return body->IsActive(); }
			bool IsStatic () const { return body->IsStatic(); }
			bool IsKinematic () const { return body->IsKinematic(); }
			bool IsDynamic () const { return body->IsDynamic(); }
			float GetFriction () const { return body->GetFriction(); }
			void SetFriction (float inFriction) { return body->SetFriction(inFriction); }
			Utilities::Vector3d GetLinearVelocity () const { return body->GetLinearVelocity(); }
			void SetLinearVelocity (Utilities::Vector3d inLinearVelocity) { return body->SetLinearVelocity(inLinearVelocity); }
			void SetLinearVelocityClamped (Utilities::Vector3d inLinearVelocity) { return body->SetLinearVelocityClamped(inLinearVelocity); }
			Utilities::Vector3d GetAngularVelocity () const { return body->GetAngularVelocity(); }
			void SetAngularVelocity (Utilities::Vector3d inAngularVelocity) { return body->SetAngularVelocity(inAngularVelocity); }
			void SetAngularVelocityClamped (Utilities::Vector3d inAngularVelocity) { return body->SetAngularVelocityClamped(inAngularVelocity); }
			Utilities::Vector3d GetPointVelocityCOM (Utilities::Vector3d inPointRelativeToCOM) const { return body->GetPointVelocityCOM(inPointRelativeToCOM); }
			Utilities::Vector3d GetPointVelocity (Utilities::Vector3d inPoint) const { return body->GetPointVelocity(inPoint); }
			void AddForce (Utilities::Vector3d inForce) { return body->AddForce(inForce); }
			void AddForce (Utilities::Vector3d inForce, Utilities::Vector3d inPosition) { return body->AddForce(inForce, inPosition); }
			void AddTorque (Utilities::Vector3d inTorque) { return body->AddTorque(inTorque); }
			void AddImpulse (Utilities::Vector3d inImpulse) { return body->AddImpulse(inImpulse); }
			void AddImpulse (Utilities::Vector3d inImpulse, Utilities::Vector3d inPosition) { return body->AddImpulse(inImpulse, inPosition); }
			void AddAngularImpulse (Utilities::Vector3d inAngularImpulse) { return body->AddAngularImpulse(inAngularImpulse); }
			void MoveKinematic (Utilities::Vector3d inTargetPosition, Utilities::Rotation3d inTargetRotation, float inDeltaTime) { return body->MoveKinematic(inTargetPosition, inTargetRotation, inDeltaTime); }
	};
}