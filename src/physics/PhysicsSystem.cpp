#ifdef StevEngine_PHYSICS
#include "PhysicsSystem.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "utilities/Vector3.hpp"

#include <math.h>

#include <Jolt/Core/Memory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Collision/CastResult.h>


namespace StevEngine::Physics {
	using namespace JPH;
	using namespace std;

	//Tick
	void PhysicsSystem::Update(double deltaTime) {
		joltSystem.Update(deltaTime, 1, &tempAllocator, &jobSystem);
	}

	//Constructor
	PhysicsSystem physics = PhysicsSystem();
	JPH::PhysicsSystem CreateJoltSystem() {
		RegisterDefaultAllocator();
		return JPH::PhysicsSystem();
	}
	PhysicsSystem::PhysicsSystem() : joltSystem(CreateJoltSystem()) {}

	//Start
	void PhysicsSystem::Init(JPH::PhysicsSettings settings) {
		// Create a factory and register Jolt physics Types
		Factory::sInstance = new Factory();
		RegisterTypes();
		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		const uint32_t cMaxBodies = 65536;
		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		const uint32_t cNumBodyMutexes = 0;
		// This is the max amount of body pairs that can be queued at any time
		const uint32_t cMaxBodyPairs = 65536;
		// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		const uint32_t cMaxContactConstraints = 10240;
		//Initialize job system
		jobSystem.Init(1024);
		// Create the actual physics system.
		joltSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);
		// Set system settings
		joltSystem.SetPhysicsSettings(settings);
		joltSystem.SetGravity(Utilities::Vector3::up * (-9.815));
		//Events
		engine->GetEvents().Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); });
	}

	JPH::Body* PhysicsSystem::CreateBody(JPH::BodyCreationSettings settings, RigidBody* attachedRigidBody) {
		JPH::Body* body = joltSystem.GetBodyInterface().CreateBody(settings);
		joltSystem.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
		rigidBodies.emplace(body->GetID(), attachedRigidBody);
		return body;
	}

	void PhysicsSystem::DestroyBody(JPH::Body* body, RigidBody* attachedRigidBody) {
		rigidBodies.erase(body->GetID());
		joltSystem.GetBodyInterface().DestroyBody(body->GetID());
	}

	RigidBody* PhysicsSystem::CastRay(JPH::RayCast ray, Utilities::Vector3* hitPoint) const {
		JPH::RayCastResult result;
		if (joltSystem.GetNarrowPhaseQuery().CastRay(JPH::RRayCast(ray), result)) {
			if(hitPoint) *hitPoint = ray.GetPointOnRay(result.mFraction);
			return rigidBodies.at(result.mBodyID); // Return hit rigid body component
		} else {
			return nullptr;
		}
	};
}
#endif
