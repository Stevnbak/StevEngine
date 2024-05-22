#pragma once
#include "GameObject.hpp"
#include "Utilities.hpp"
#include "Colliders.hpp"

//Jolt imports
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace StevEngine::Physics {
	//Constraints
	class Constraints {
		public:
			bool FreezePositionX = false;	//Freeze physics motion along the X - axis.
			bool FreezePositionY = false;	//Freeze physics motion along the Y - axis.
			bool FreezePositionZ = false;	//Freeze physics motion along the Z - axis.
			bool FreezeRotationX = false;	//Freeze physics rotation along the X - axis.
			bool FreezeRotationY = false;	//Freeze physics rotation along the Y - axis.
			bool FreezeRotationZ = false;	//Freeze physics rotation along the Z - axis.
			void FreezePosition();
			void FreezeRotation();
	};
	//Physics component
	class PhysicsBody : public Component {
		//Information
		public:
			//Basic values
			Constraints constraints = Constraints();
			Utilities::Vector3d centerOfMassOffset = Utilities::Vector3d(0, 0, 0);
			double mass = 1;
			//Gravity values
			bool isAffectedByGravity = true;
			double gravityAcceleration = 9.82;
			Utilities::Vector3d gravityDirection = Utilities::Vector3d(0, -1, 0);
			//Surface values
			double frictionConstant = 1;
			double bounceConstant = 1;
			//Drag values
			double dragConstant = 1;
			//Limits
			double maxVelocity = -1;
			double maxAcceleration = -1;
			double maxAngularVelocity = -1;
			double maxAngularAcceleration = -1;
		private:
			JPH::EMotionType motionType;
			JPH::ObjectLayer layer;
			std::vector<Collider*> colliders;
			JPH::Body* body;
			JPH::Ref<JPH::Shape> shape;
			Utilities::Vector3d velocity = Utilities::Vector3d(0, 0, 0);
			Utilities::Vector3d acceleration = Utilities::Vector3d(0, 0, 0);
			Utilities::Vector3d angularVelocity = Utilities::Vector3d(0, 0, 0);
			Utilities::Vector3d angularAcceleration = Utilities::Vector3d(0, 0, 0);
		//Functions
		public:
			//Basic component functions
			void Start();
			void Update(double deltaTime);
			void Draw() {}
			void Destroy();
			//Constructor
			PhysicsBody(JPH::EMotionType motionType, JPH::ObjectLayer layer);
			//Force functions
			void AddForce(Utilities::Vector3d force);
			void AddImpulseForce(Utilities::Vector3d force, double time = 1);
			void AddForceAtPoint(Utilities::Vector3d force, Utilities::Vector3d point);
			//Other functions
			void UpdateColliders();
		private:
			std::vector<std::pair<Utilities::Vector3d, double>> activeImpulseForces;
			void UpdateCollisions();
			void UpdateImpulseForces(double deltaTime);
			void Gravity();
			void Drag();
			void ResetAcceleration();
	};
}