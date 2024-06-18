#pragma once
#include <core/GameObject.hpp>
#include <core/Utilities.hpp>
//Jolt:
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

namespace StevEngine::Physics {

	//Base collider
	class Collider : public Component {
		public:
			Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
			Collider(tinyxml2::XMLElement* element);
			void Draw();
			void Update(double deltaTime) {};
			void Start();
			void Destroy();
			void Export(tinyxml2::XMLElement* element);
			void TransformUpdate(bool position, bool rotation, bool scale);

			static const bool unique = false;

			JPH::Ref<JPH::Shape> shape;

			Utilities::Vector3 getScale()  { return scale; }
			Utilities::Quaternion getRotation()  { return rotation; }
			Utilities::Vector3 getPosition() { return position; }
			Utilities::Range3 getBounds() { return shape->GetLocalBounds(); };
			Utilities::Vector3 getCenterOfMass() { return shape->GetCenterOfMass(); };
		protected:
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			const JPH::Ref<JPH::Shape> rawShape;
	};

	//Cube collider
	class CubeCollider : public Collider {
		public:
			CubeCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	//Sphere collider
	class SphereCollider : public Collider {
		public:
			SphereCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	//Cylinder collider
	class CylinderCollider : public Collider {
		public:
			CylinderCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	//Capsule collider
	class CapsuleCollider : public Collider {
		public:
			CapsuleCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};
}