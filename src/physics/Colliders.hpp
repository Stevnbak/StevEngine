#pragma once
#ifdef StevEngine_PHYSICS
#include "main/EventSystem.hpp"
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
	/**
	 * @brief Base class for all collision shapes
	 *
	 * Provides common functionality for physics collision shapes.
	 * Handles transform updates and shape creation/modification.
	 * Integrates with the Jolt physics engine for collision detection.
	 */
	class Collider : public Component {
		friend class StevEngine::GameObject;
		public:
			/**
			 * @brief Create new collider
			 * @param shape Jolt physics shape
			 * @param position Local position offset
			 * @param rotation Local rotation offset
			 * @param scale Local scale modifier
			 */
			Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));

			/**
			 * @brief Create collider from serialized data
			 * @param node YAML node with collider data
			 */
			Collider(YAML::Node);

			~Collider();

			/** @brief Get local scale */
			Utilities::Vector3 GetScale() const { return scale; }
			/** @brief Get local rotation */
			Utilities::Quaternion GetRotation() const  { return rotation; }
			/** @brief Get local position */
			Utilities::Vector3 GetPosition() const { return position; }

			/**
			 * @brief Set local scale
			 * @param scale New scale vector
			 */
			void SetScale(Utilities::Vector3 scale);

			/**
			 * @brief Set local rotation
			 * @param rotation New rotation quaternion
			 */
			void SetRotation(Utilities::Quaternion rotation);

			/**
			 * @brief Set local position
			 * @param position New position vector
			 */
			void SetPosition(Utilities::Vector3 position);

			/**
			 * @brief Set full local transform
			 * @param position New position
			 * @param rotation New rotation
			 * @param scale New scale
			 */
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale);

			/**
			 * @brief Get axis-aligned bounds
			 * @return Bounding box in local space
			 */
			Utilities::Range3 GetBounds() const { return shape->GetLocalBounds(); };

			/**
			 * @brief Get center of mass
			 * @return Center of mass in local space
			 */
			Utilities::Vector3 GetCenterOfMass() const { return shape->GetCenterOfMass(); };

			/**
			 * @brief Get physics shape
			 * @return Reference to Jolt shape
			 */
			JPH::Ref<JPH::Shape> GetShape() const { return shape; }

			/**
			 * @brief Serialize collider to YAML
			 * @param node YAML node to serialize into
			 * @return Updated YAML node containing collider data
			 */
			YAML::Node Export(YAML::Node node) const;

		protected:
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);   ///< Local scale
			Utilities::Vector3 position = Utilities::Vector3();		///< Local position
			Utilities::Quaternion rotation = Utilities::Quaternion();  ///< Local rotation
			const JPH::Ref<JPH::Shape> rawShape;					  ///< Base physics shape
			JPH::Ref<JPH::Shape> shape;							   ///< Transformed shape

		private:
			/**
			 * @brief Clean up collider when component is deactivated
			 * Releases Jolt physics shape resources
			 */
			void Deactivate();

			/**
			 * @brief Initialize collider after creation
			 * Sets up correct scale for shape and subscribes to transform events
			 */
			void Start();

			/**
			 * @brief Handle transform changes
			 * Updates physics shape when transform changes
			 * @param position Whether position changed
			 * @param rotation Whether rotation changed
			 * @param scale Whether scale changed
			 * @param fromLocal Whether change was local or from parent
			 */
			void TransformUpdate(bool position, bool rotation, bool scale, bool fromLocal = false);
	};

	/**
	 * @brief Box-shaped collider
	 *
	 * Represents a rectangular box collision shape.
	 * Uses Jolt's BoxShape internally.
	 */
	class CubeCollider : public Collider {
		public:
			/**
			 * @brief Create box collider
			 * @param position Local position
			 * @param rotation Local rotation
			 * @param scale Local scale
			 */
			CubeCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	/**
	 * @brief Sphere-shaped collider
	 *
	 * Represents a spherical collision shape.
	 * Uses Jolt's SphereShape internally.
	 */
	class SphereCollider : public Collider {
		public:
			/**
			 * @brief Create sphere collider
			 * @param position Local position
			 * @param rotation Local rotation
			 * @param scale Local scale
			 */
			SphereCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	/**
	 * @brief Cylinder-shaped collider
	 *
	 * Represents a cylindrical collision shape.
	 * Uses Jolt's CylinderShape internally.
	 */
	class CylinderCollider : public Collider {
		public:
			/**
			 * @brief Create cylinder collider
			 * @param position Local position
			 * @param rotation Local rotation
			 * @param scale Local scale
			 */
			CylinderCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	/**
	 * @brief Capsule-shaped collider
	 *
	 * Represents a capsule collision shape (cylinder with hemispherical ends).
	 * Uses Jolt's CapsuleShape internally.
	 */
	class CapsuleCollider : public Collider {
		public:
			/**
			 * @brief Create capsule collider
			 * @param position Local position
			 * @param rotation Local rotation
			 * @param scale Local scale
			 */
			CapsuleCollider(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	/**
	 * @brief Mesh-based collider
	 *
	 * Creates collision shape from 3D mesh data.
	 * Can create either convex hull or concave mesh collider.
	 */
	class ModelCollider : public Collider {
		public:
			/**
			 * @brief Create mesh collider
			 * @param model Model to generate collision from
			 * @param convex Whether to create convex hull
			 * @param position Local position
			 * @param rotation Local rotation
			 * @param scale Local scale
			 */
			ModelCollider(const Utilities::Model& model, bool convex = true, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	/**
	 * @brief Heightmap terrain collider
	 *
	 * Creates collision shape from heightmap terrain data.
	 * Uses Jolt's HeightFieldShape internally.
	 */
	class TerrainCollider : public Collider {
		public:
			/**
			 * @brief Create terrain collider
			 * @param data Heightmap terrain data
			 * @param position Local position
			 * @param rotation Local rotation
			 * @param scale Local scale
			 */
			TerrainCollider(const Utilities::TerrainData& data, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
	};

	/**
	 * @brief Event for collider updates
	 *
	 * Triggered when a collider's transform is modified.
	 */
	class ColliderUpdateEvent : public Event {
		public:
			/**
			 * @brief Create collider update event
			 * @param collider Modified collider
			 */
			ColliderUpdateEvent(Collider* collider) : collider(collider) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "ColliderUpdateEvent"; }
			Collider* collider;  ///< Modified collider
	};
}
#endif