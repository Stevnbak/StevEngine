#pragma once
#ifdef StevEngine_PHYSICS
#include "main/Component.hpp"
#include "physics/Layers.hpp"
#include "utilities/Vector3.hpp"

#include "Jolt/Physics/Character/CharacterVirtual.h"

#define CHARACTERBODY_TYPE "CharacterBody"

namespace StevEngine::Physics {
	struct CharacterSettings {
		float mass = 70;							///< Character mass (kg)
		float maxStrength = 100;					///< Maximum force with which the character can push other bodies (N).
		float predictiveContactDistance = 0.1f;		///< How far to scan outside of the shape for predictive contacts. A value of 0 will most likely cause the character to get stuck as it cannot properly calculate a sliding direction anymore. A value that's too high will cause ghost collisions.
		uint32_t maxCollisionIterations = 5;		///< Max amount of collision loops
		uint32_t maxConstraintIterations = 15;		///< How often to try stepping in the constraint solving
		float minTimeRemaining = 1.0e-4f;			///< Early out condition: If this much time is left to simulate we are done
		float collisionTolerance = 1.0e-3f;			///< How far we're willing to penetrate geometry
		float characterPadding = 0.02f; 			///< How far we try to stay away from the geometry, this ensures that the sweep will hit as little as possible lowering the collision cost and reducing the risk of getting stuck
		uint32_t maxNumHits = 256; 					///< Max num hits to collect in order to avoid excess of contact points collection
		float hitReductionCosMaxAngle = 0.999f;		///< Cos(angle) where angle is the maximum angle between two hits contact normals that are allowed to be merged during hit reduction. Default is around 2.5 degrees. Set to -1 to turn off.
		float penetrationRecoverySpeed = 1.0f;		///< This value governs how fast a penetration will be resolved, 0 = nothing is resolved, 1 = everything in one update
	};

	/**
	 * @brief Physics character body component
	 *
	 * Adds physical objects that are simulated separately from the rest of the Jolt physics rigid bodies.
	 * Handles collision shapes, forces, and motion simulation.
	 */
	class CharacterBody : public Component {
		public:
			static const bool unique = true;			///< Only one per GameObject
			/**
			 * @brief Create rigid body
			 * @param settings Character controller settings
			 * @param layer Collision layer
			 * @param mass Mass in kg
			 */
			CharacterBody(CharacterSettings settings = {}, LayerID layer = LayerManager::DEFAULT);

			/**
			 * @brief Create rigidbody from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			CharacterBody(Utilities::Stream& stream);

			Utilities::Vector3 velocity; ///< Character velocity

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return CHARACTERBODY_TYPE; }

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
			 * Adds gravity to the current velocity
			 * @param deltaTime Time since last update
			 */
			void Update(double deltaTime);

			/**
			 * @brief Extended physics transform update
			 * @param deltaTime Time since last update
			 */
			void ExtendedUpdate(double deltaTime, JPH::CharacterVirtual::ExtendedUpdateSettings updateSettings = {});

			/**
			 * @brief Clean up resources
			 */
			~CharacterBody();

			/**
			 * @brief Refresh combined collision shape
			 * Rebuilds shape from all attached colliders
			 */
			void RefreshShape();

		protected:
			JPH::CharacterVirtualSettings settings;		///< Jolt Physics Virtual Character Controller Settings
			JPH::CharacterVirtual* jphCharacter;			///< Jolt Physics Virtual Character Controller

			LayerID layer; 								///< Physics layer

		private:
			JPH::Ref<JPH::Shape> shape;					///< Combined collision shape

			// Jolt filters
			JPH::BodyFilter bodyFilter;
			JPH::ShapeFilter shapeFilter;

		// Function wrappers
		public:
			bool 	IsSupported() const { return jphCharacter->IsSupported(); }; ///< Is character supported (by ground/wall)
			bool 	IsOnGround() const { return jphCharacter->GetGroundState() == JPH::CharacterBase::EGroundState::OnGround; }; ///< Is character on the ground
			float 	GetMass () const { return jphCharacter->GetMass(); }; ///< Get character mass (kg)
			void 	SetMass (float mass) { jphCharacter->SetMass(mass); settings.mMass = mass; }; ///< Set character mass (kg)
			float 	GetMaxStrength () const { return jphCharacter->GetMaxStrength(); }; ///< Maximum force with which the character can push other bodies (N)
			void 	SetMaxStrength (float maxStrength) { jphCharacter->SetMaxStrength(maxStrength); settings.mMaxStrength = maxStrength; };
			float 	GetPenetrationRecoverySpeed () const { return jphCharacter->GetPenetrationRecoverySpeed(); }; ///< This value governs how fast a penetration will be resolved, 0 = nothing is resolved, 1 = everything in one update.
			void 	SetPenetrationRecoverySpeed (float speed) { jphCharacter->SetPenetrationRecoverySpeed(speed); settings.mPenetrationRecoverySpeed = speed; }; ///< Set how fast a penetration will be resolved, 0 = nothing is resolved, 1 = everything in one update.
			float 	GetCharacterPadding () const  { return jphCharacter->GetCharacterPadding(); };///< Character padding.
			uint 	GetMaxNumHits () const  { return jphCharacter->GetMaxNumHits(); };///< Max num hits to collect in order to avoid excess of contact points collection.
			void 	SetMaxNumHits (uint maxHits) { jphCharacter->SetMaxNumHits(maxHits); settings.mMaxNumHits = maxHits; }; ///< Set max num hits to collect in order to avoid excess of contact points collection.
			float 	GetHitReductionCosMaxAngle () const { return jphCharacter->GetHitReductionCosMaxAngle(); }; ///< Cos(angle) where angle is the maximum angle between two hits contact normals that are allowed to be merged during hit reduction.
    		void 	SetHitReductionCosMaxAngle (float cosMaxAngle) { jphCharacter->SetHitReductionCosMaxAngle(cosMaxAngle); settings.mHitReductionCosMaxAngle = cosMaxAngle; }; ///< Set Cos(angle) where angle is the maximum angle between two hits contact normals that are allowed to be merged during hit reduction. Default is around 2.5 degrees. Set to -1 to turn off.
      		void 	SetMaxSlopeAngle (float maxSlopeAngle) { jphCharacter->SetMaxSlopeAngle(maxSlopeAngle); settings.mMaxSlopeAngle = maxSlopeAngle; }; ///< Set the maximum angle of slope that character can still walk on (radians)
       		bool 	IsSlopeTooSteep (Utilities::Vector3 normal) const { return jphCharacter->IsSlopeTooSteep(normal); }; ///< Check if the normal of the ground surface is too steep to walk on.
	};
}
#endif