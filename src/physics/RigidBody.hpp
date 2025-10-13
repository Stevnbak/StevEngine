#pragma once
#ifdef StevEngine_PHYSICS
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "physics/Layers.hpp"
#include "utilities/Stream.hpp"

//Jolt imports
#include "Jolt.h"
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#define RIGIDBODY_TYPE "RigidBody"

namespace StevEngine::Physics {
	/**
	 * @brief Motion properties configuration for rigid bodies
	 */
	struct MotionProperties {
		double LinearDamping;		  ///< Linear velocity damping factor
		double AngularDamping;		 ///< Angular velocity damping factor
		double MaxLinearVelocity;	  ///< Maximum linear velocity
		double MaxAngularVelocity;	 ///< Maximum angular velocity
		double GravityFactor;		  ///< Gravity influence multiplier
		JPH::EAllowedDOFs AllowedDOFs; ///< Allowed degrees of freedom

		/**
		 * @brief Create motion properties configuration
		 * @param gravityFactor Gravity influence multiplier
		 * @param linearDamping Linear velocity damping
		 * @param angularDamping Angular velocity damping
		 * @param allowedDOFs Allowed degrees of freedom
		 * @param maxLinearVelocity Maximum linear velocity (0 for unlimited)
		 * @param maxAngularVelocity Maximum angular velocity (0 for unlimited)
		 */
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

	/**
	 * @brief Physics rigid body component
	 *
	 * Adds physical simulation to game objects using Jolt physics.
	 * Handles collision shapes, forces, and motion simulation.
	 */
	class RigidBody : public Component {
		public:
			JPH::Body* GetBody() const { return body; }	 ///< Get Jolt physics body
			const LayerID layer;							 ///< Physics collision layer
			const JPH::EMotionType motionType;			  ///< Motion type (static/dynamic/kinematic)
			const float mass;							   ///< Body mass in kg
			static const bool unique = true;				///< Only one per GameObject

		private:
			MotionProperties motionProperties;			  ///< Motion behavior settings
			JPH::Body* body;							   ///< Jolt physics body
			JPH::Ref<JPH::Shape> shape;					///< Combined collision shape

		public:
			/**
			 * @brief Create rigid body
			 * @param motionType Body motion type
			 * @param layer Collision layer
			 * @param mass Mass in kg
			 */
			RigidBody(JPH::EMotionType motionType, LayerID layer = LayerManager::DEFAULT, float mass = 1);

			/**
			 * @brief Create rigidbody from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			RigidBody(Utilities::Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return RIGIDBODY_TYPE; }

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Utilities::Stream Export(Utilities::StreamType type) const;

			/**
			 * @brief Initialize component
			 */
			void Start();

			/**
			 * @brief Clean up when deactivated
			 */
			void Deactivate();

			/**
			 * @brief Update transform from physics
			 * @param deltaTime Time since last update
			 */
			void Update(double deltaTime);

			/**
			 * @brief Clean up resources
			 */
			~RigidBody();

			/**
			 * @brief Set motion properties
			 * @param properties New motion settings
			 */
			void SetMotionProperties(MotionProperties properties);

		private:
			/**
			 * @brief Refresh combined collision shape
			 * Rebuilds shape from all attached colliders
			 */
			void RefreshShape();

			/**
			 * @brief Handle transform updates
			 * @param position Whether position changed
			 * @param rotation Whether rotation changed
			 * @param scale Whether scale changed
			 */
			void TransformUpdate(bool position, bool rotation, bool scale);

		// Jolt physics body wrapper functions
		public:
		/**
         * @brief Check if body is currently active in the physics simulation
         * @return True if the body is active
         */
        bool IsActive() const { return body->IsActive(); }

        /**
         * @brief Check if this is a static body
         * @return True if this is a static (non-moving) body
         */
        bool IsStatic() const { return body->IsStatic(); }

        /**
         * @brief Check if this is a kinematic body
         * @return True if this is a kinematic (animated) body
         */
        bool IsKinematic() const { return body->IsKinematic(); }

        /**
         * @brief Check if this is a dynamic body
         * @return True if this is a dynamic (simulated) body
         */
        bool IsDynamic() const { return body->IsDynamic(); }

        /**
         * @brief Get the friction coefficient
         * @return Friction coefficient [0, INF]
         */
        float GetFriction() const { return body->GetFriction(); }

        /**
         * @brief Set the friction coefficient
         * @param inFriction New friction value [0, INF]
         */
        void SetFriction(float inFriction) { return body->SetFriction(inFriction); }

        /**
         * @brief Get the current linear velocity
         * @return Linear velocity in world space
         */
        Utilities::Vector3 GetLinearVelocity() const { return body->GetLinearVelocity(); }

        /**
         * @brief Set the linear velocity
         * @param inLinearVelocity New velocity in world space
         */
        void SetLinearVelocity(Utilities::Vector3 inLinearVelocity) { return body->SetLinearVelocity(inLinearVelocity); }

        /**
         * @brief Set linear velocity clamped to the maximum configured value
         * @param inLinearVelocity Desired velocity in world space
         */
        void SetLinearVelocityClamped(Utilities::Vector3 inLinearVelocity) { return body->SetLinearVelocityClamped(inLinearVelocity); }

        /**
         * @brief Get the current angular velocity
         * @return Angular velocity in world space (radians/s)
         */
        Utilities::Vector3 GetAngularVelocity() const { return body->GetAngularVelocity(); }

        /**
         * @brief Set the angular velocity
         * @param inAngularVelocity New angular velocity in world space (radians/s)
         */
        void SetAngularVelocity(Utilities::Vector3 inAngularVelocity) { return body->SetAngularVelocity(inAngularVelocity); }

        /**
         * @brief Set angular velocity clamped to the maximum configured value
         * @param inAngularVelocity Desired angular velocity in world space (radians/s)
         */
        void SetAngularVelocityClamped(Utilities::Vector3 inAngularVelocity) { return body->SetAngularVelocityClamped(inAngularVelocity); }

        /**
         * @brief Get velocity of a point relative to center of mass
         * @param inPointRelativeToCOM Point relative to center of mass
         * @return Velocity at the specified point
         */
        Utilities::Vector3 GetPointVelocityCOM(Utilities::Vector3 inPointRelativeToCOM) const { return body->GetPointVelocityCOM(inPointRelativeToCOM); }

        /**
         * @brief Get velocity of a point in world space
         * @param inPoint Point in world space
         * @return Velocity at the specified point
         */
        Utilities::Vector3 GetPointVelocity(Utilities::Vector3 inPoint) const { return body->GetPointVelocity(inPoint); }

        /**
         * @brief Add force to center of mass
         * @param inForce Force vector in world space
         */
        void AddForce(Utilities::Vector3 inForce) { return body->AddForce(inForce); }

        /**
         * @brief Add force at specified world space position
         * @param inForce Force vector in world space
         * @param inPosition Position to apply force in world space
         */
        void AddForce(Utilities::Vector3 inForce, Utilities::Vector3 inPosition) { return body->AddForce(inForce, inPosition); }

        /**
         * @brief Add torque around center of mass
         * @param inTorque Torque vector in world space
         */
        void AddTorque(Utilities::Vector3 inTorque) { return body->AddTorque(inTorque); }

        /**
         * @brief Add linear impulse to center of mass
         * @param inImpulse Impulse vector in world space
         */
        void AddImpulse(Utilities::Vector3 inImpulse) { return body->AddImpulse(inImpulse); }

        /**
         * @brief Add linear impulse at specified world space position
         * @param inImpulse Impulse vector in world space
         * @param inPosition Position to apply impulse in world space
         */
        void AddImpulse(Utilities::Vector3 inImpulse, Utilities::Vector3 inPosition) { return body->AddImpulse(inImpulse, inPosition); }

        /**
         * @brief Add angular impulse around center of mass
         * @param inAngularImpulse Angular impulse vector in world space
         */
        void AddAngularImpulse(Utilities::Vector3 inAngularImpulse) { return body->AddAngularImpulse(inAngularImpulse); }

        /**
         * @brief Move kinematic body towards target transform
         * @param inTargetPosition Target position in world space
         * @param inTargetRotation Target rotation in world space
         * @param inDeltaTime Time step for the movement
         */
        void MoveKinematic(Utilities::Vector3 inTargetPosition, Utilities::Quaternion inTargetRotation, float inDeltaTime) { return body->MoveKinematic(inTargetPosition, inTargetRotation, inDeltaTime); }
	};
}
#endif