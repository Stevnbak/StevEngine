#pragma once
#include "GameObject.hpp"
#include "Utilities.hpp"
namespace StevEngine::Physics {

	//Base collider
	class Collider : public Component {
		public:
			Utilities::Range3d CalculateRange();
			virtual bool IsPointInCollider(Utilities::Vector3d point) = 0;
			virtual bool IsOverlapping(Collider* other) = 0;
			virtual Utilities::Vector3d CollisionPoint(Collider* other) = 0;
			void Draw() {};
			void Update(double deltaTime) {};
			void Start();
			//Basic properties
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Vector3d rotation = Utilities::Vector3d();
			Utilities::Vector3d scale = Utilities::Vector3d(1, 1, 1);
			bool unique = false;
		protected:
			bool IsInRange(Collider* other);
	};

	//Cube collider
	class CubeCollider : public Collider {
		public:
			//Functions
			bool IsPointInCollider(Utilities::Vector3d point);
			bool IsOverlapping(Collider* other);
			Utilities::Vector3d CollisionPoint(Collider* other);
	};
}