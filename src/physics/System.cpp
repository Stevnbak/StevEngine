#include "System.hpp"
#include <core/Log.hpp>

#include <math.h>
#include <iostream>
#include <cstdarg>

#include <Jolt/Core/Memory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

// Callback for traces
static void TraceImpl(const char *inFMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);
	// Print to the TTY
	Log::Warning(buffer);
}

namespace StevEngine::Physics {
	using namespace JPH;
	using namespace std;

	//Tick
	void System::Update(double deltaTime) {
		joltSystem.Update(deltaTime, 1, &tempAllocator, &jobSystem);
	}

	//Start
	System::System(JPH::PhysicsSettings settings) {
		// Register allocation hook.
		RegisterDefaultAllocator();
		// Install trace and assert callbacks
		Trace = TraceImpl;
		// Create a factory and register Jolt physics Types
		Factory::sInstance = new Factory();
		RegisterTypes();
		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		const uint cMaxBodies = 65536;
		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		const uint cNumBodyMutexes = 0;
		// This is the max amount of body pairs that can be queued at any time
		const uint cMaxBodyPairs = 65536;
		// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		const uint cMaxContactConstraints = 10240;
		//Initialize job system
		jobSystem.Init(1024);
		// Create default layers
		new Layer("Moving", false);
		new Layer("Static", true);
		// Create the actual physics system.
		joltSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);
		// Set system settings
		joltSystem.SetPhysicsSettings(settings);
		//Get The body interface
		bodyInterface = &joltSystem.GetBodyInterface();
	}
}
