#pragma once
#include "GameObject.hpp"
#include "Utilities.hpp"
#include "Colliders.hpp"

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
	class Physics : Component {
		//Information
		public:
			//Basic values
			bool isStatic = false;
			Constraints constraints = Constraints();
			Utilities::Vector3d centerOfMassOffset = Utilities::Vector3d(0, 0, 0);
			double mass = 1;
			//Gravity values
			bool isAffectedByGravity = true;
			double gravityAcceleration = 9.82;
			Utilities::Vector3d gravityDirection = Utilities::Vector3d(0, 0, -1);
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
			//Current values
			Utilities::Vector3d velocity = Utilities::Vector3d(0, 0, 0);
			Utilities::Vector3d acceleration = Utilities::Vector3d(0, 0, 0);
			Utilities::Vector3d angularVelocity = Utilities::Vector3d(0, 0, 0);
			Utilities::Vector3d angularAcceleration = Utilities::Vector3d(0, 0, 0);
			std::vector<Collider*> colliders;
			long lastColliderUpdate = 0;
		//Functions
		public:
			//Basic component functions
			void Start();
			void Update(double deltaTime);
			void Draw() {}
			//Constructor
			Physics();
			//Force functions
			void AddForce(Utilities::Vector3d force);
			void AddForceAtPoint(Utilities::Vector3d force, Utilities::Vector3d point);
			//Other functions
			void UpdateColliders();
	private:
			void UpdateCollisions();
			void Gravity();
			void Drag();
			void ResetAcceleration();
	};
}