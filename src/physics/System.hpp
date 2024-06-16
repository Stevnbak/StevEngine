#pragma once
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

namespace StevEngine::Physics {
	
	//Main system
	class System {
		public:
			System(JPH::PhysicsSettings settings = JPH::PhysicsSettings());
			JPH::BodyInterface* bodyInterface;
			void Update(double deltaTime);
		private:
			//Jolt system
			JPH::PhysicsSystem joltSystem;
			// Memory Allocator
			JPH::TempAllocatorMalloc tempAllocator = JPH::TempAllocatorMalloc();
			// Job System
			JPH::JobSystemSingleThreaded jobSystem = JPH::JobSystemSingleThreaded();
			// Create mapping table from object layer to broadphase layer
			BPLayerInterfaceImpl broad_phase_layer_interface = BPLayerInterfaceImpl();
			// Create class that filters object vs broadphase layers
			ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter = ObjectVsBroadPhaseLayerFilterImpl();
			// Create class that filters object vs object layers
			ObjectLayerPairFilterImpl object_vs_object_layer_filter = ObjectLayerPairFilterImpl();
	};
}