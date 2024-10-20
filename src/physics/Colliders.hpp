#pragma once
#ifdef StevEngine_PHYSICS
#include "main/EventSystem.hpp"
#include "main/ResourceManager.hpp"
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Range3.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Model.hpp"
#include "utilities/Terrain.hpp"
#include <yaml-cpp/yaml.h>
//Jolt:
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

namespace StevEngine::Physics {

	//Base collider
	class Collider : public Component {
		friend class StevEngine::GameObject;
		public:
			Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
			Collider(YAML::Node);
			~Collider();
		private:
			void Deactivate();
			void Start();
			void TransformUpdate(bool position, bool rotation, bool scale, bool fromLocal = false);
		public:
			YAML::Node Export(YAML::Node node) const;
			Utilities::Vector3 GetScale() const { return scale; }
			Utilities::Quaternion GetRotation() const  { return rotation; }
			Utilities::Vector3 GetPosition() const { return position; }
			void SetScale(Utilities::Vector3 scale);
			void SetRotation(Utilities::Quaternion rotation);
			void SetPosition(Utilities::Vector3 position);
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale);
			Utilities::Range3 GetBounds() const { return shape->GetLocalBounds(); };
			Utilities::Vector3 GetCenterOfMass() const { return shape->GetCenterOfMass(); };
			JPH::Ref<JPH::Shape> GetShape() const { return shape; }
		protected:
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			const JPH::Ref<JPH::Shape> rawShape;
			JPH::Ref<JPH::Shape> shape;
	};
	inline bool collider = CreateComponents::RegisterComponentType<Collider>("Collider");

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

	//Model collider
	class ModelCollider : public Collider {
		public:
			ModelCollider(const Utilities::Model& model, bool convex = true, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	//Terrain collider
	class TerrainCollider : public Collider {
		public:
			TerrainCollider(const Utilities::TerrainData& data, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	//Collider events
	class ColliderUpdateEvent : public Event {
		public:
			ColliderUpdateEvent(Collider* collider) : collider(collider) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "ColliderUpdateEvent"; }
			Collider* collider;
	};
}
#endif
