#pragma once
#ifdef StevEngine_PHYSICS
#include "Layers.hpp"

//Jolt imports
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

namespace StevEngine {
	class Engine;
	namespace Physics {
		/**
		 * @brief Core physics simulation system
		 *
		 * Handles initialization and updating of the Jolt physics engine.
		 * Manages physics settings, bodies, and simulation state.
		 */
		class PhysicsSystem {
			friend class StevEngine::Engine;
			public:
				/**
				 * @brief Create physics system
				 */
				PhysicsSystem();

				/**
				 * @brief Initialize physics system
				 * @param settings Jolt physics settings
				 */
				void Init(JPH::PhysicsSettings settings);

				/**
				 * @brief Get physics body interface
				 * @return Pointer to Jolt body interface
				 */
				JPH::BodyInterface* GetBodyInterface() const { return bodyInterface; }

			private:
				/**
				 * @brief Update physics simulation
				 * @param deltaTime Time step for physics simulation
				 */
				void Update(double deltaTime);

				JPH::PhysicsSystem joltSystem;		  ///< Main Jolt physics system
				JPH::BodyInterface* bodyInterface;	   ///< Interface for manipulating physics bodies
				JPH::TempAllocatorMalloc tempAllocator; ///< Memory allocator for physics
				JPH::JobSystemSingleThreaded jobSystem;  ///< Job system for physics calculations

				// Layer management
				BPLayerInterfaceImpl broad_phase_layer_interface;					///< Broad phase layer interface
				ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter; ///< Broad phase collision filter
				ObjectLayerPairFilterImpl object_vs_object_layer_filter;			 ///< Object pair collision filter
		};

		extern PhysicsSystem physics; ///< Global physics system instance
	}
}
#endif
