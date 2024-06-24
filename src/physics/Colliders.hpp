#pragma once
#include <scenes/Component.hpp>
#include <utilities/Vector3.hpp>
#include <utilities/Range3.hpp>
#include <utilities/Quaternion.hpp>
//Jolt:
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

namespace StevEngine::Physics {

	//Base collider
	class Collider : public Component {
		friend class StevEngine::GameObject;
		public:
			Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
			Collider(tinyxml2::XMLElement* element);
			~Collider();
		private:
			void Draw();
			void Deactivate();
			void Update(double deltaTime) {};
			void Start();
			void Export(tinyxml2::XMLElement* element);
			void TransformUpdate(bool position, bool rotation, bool scale);
		public:
			Utilities::Vector3 GetScale()  { return scale; }
			Utilities::Quaternion GetRotation()  { return rotation; }
			Utilities::Vector3 GetPosition() { return position; }
			void SetScale(Utilities::Vector3 scale);
			void SetRotation(Utilities::Quaternion rotation);
			void SetPosition(Utilities::Vector3 position);
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale);
			Utilities::Range3 GetBounds() { return shape->GetLocalBounds(); };
			Utilities::Vector3 GetCenterOfMass() { return shape->GetCenterOfMass(); };
			JPH::Ref<JPH::Shape> GetShape() { return shape; }
		protected:
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			const JPH::Ref<JPH::Shape> rawShape;
			JPH::Ref<JPH::Shape> shape;
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