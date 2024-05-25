#pragma once
#include <Core/GameObject.hpp>
#include <Core/Utilities.hpp>
//Jolt:
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

namespace StevEngine::Physics {

	//Base collider
	class Collider : public Component {
		public:
			Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3d position = Utilities::Vector3d(0,0,0), Utilities::Rotation3d rotation = Utilities::Rotation3d(0,0,0), Utilities::Vector3d scale = Utilities::Vector3d(1,1,1));

			void Draw();
			void Update(double deltaTime) {};
			void Start();
			void Destroy();

			bool unique = false;

			JPH::Ref<JPH::Shape> shape;

			Utilities::Vector3d getScale()  { return scale; }
			Utilities::Rotation3d getRotation()  { return rotation; }
			Utilities::Vector3d getPosition() { return position; }
		protected:
			Utilities::Vector3d scale = Utilities::Vector3d(1, 1, 1);
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Rotation3d rotation = Utilities::Rotation3d();
	};

	//Cube collider
	class CubeCollider : public Collider {
		public:
			CubeCollider(Utilities::Vector3d position = Utilities::Vector3d(0,0,0), Utilities::Rotation3d rotation = Utilities::Rotation3d(0,0,0), Utilities::Vector3d scale = Utilities::Vector3d(1,1,1));
	};

	//Sphere collider
	class SphereCollider : public Collider {
		public:
			SphereCollider(Utilities::Vector3d position = Utilities::Vector3d(0,0,0), Utilities::Rotation3d rotation = Utilities::Rotation3d(0,0,0), Utilities::Vector3d scale = Utilities::Vector3d(1,1,1));
	};

	//Cylinder collider
	class CylinderCollider : public Collider {
		public:
			CylinderCollider(Utilities::Vector3d position = Utilities::Vector3d(0,0,0), Utilities::Rotation3d rotation = Utilities::Rotation3d(0,0,0), Utilities::Vector3d scale = Utilities::Vector3d(1,1,1));
	};

	//Capsule collider
	class CapsuleCollider : public Collider {
		public:
			CapsuleCollider(Utilities::Vector3d position = Utilities::Vector3d(0,0,0), Utilities::Rotation3d rotation = Utilities::Rotation3d(0,0,0), Utilities::Vector3d scale = Utilities::Vector3d(1,1,1));
	};
}