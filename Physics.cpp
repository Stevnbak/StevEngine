#include "Physics.hpp"
#include <math.h>
#include <iostream>
#include "Log.hpp"

namespace StevEngine::Physics {
	//Basic functions
	void Physics::Start() {
		UpdateColliders();
	}
	void Physics::Update(double deltaTime) {
		//Update colliders
		if (lastColliderUpdate + 1000 > clock()) UpdateColliders();
		//Static object doesn't move
		if (isStatic) return;
		//Constant forces
		Gravity();
		Drag();
		//Collisions
		UpdateCollisions();
		//Move & Rotate object
		if (maxAcceleration >= 0 && acceleration.Magnitude() > maxAcceleration) {
			acceleration.Normalize();
			acceleration.Mult(maxAcceleration);
		}
		if (maxVelocity >= 0 && velocity.Magnitude() > maxVelocity) {
			velocity.Normalize();
			velocity.Mult(maxVelocity);
		}
		///Log::Normal(std::format("Acceleration: ({};{};{})", acceleration.X, acceleration.Y, acceleration.Z));
		///Log::Normal(std::format("Velocity: ({};{};{})", velocity.X, velocity.Y, velocity.Z));
		gameObject->position += acceleration * 0.5 * pow(deltaTime, 2) + velocity * deltaTime; // s = 1/2*a*t^2 + v_0*t + s_0
		velocity += acceleration * deltaTime; // v = a * t + v_0
		if (maxAngularAcceleration >= 0 && angularAcceleration.Magnitude() > maxAngularAcceleration) {
			angularAcceleration.Normalize();
			angularAcceleration.Mult(maxAngularAcceleration);
		}
		if (maxAngularVelocity >= 0 && angularVelocity.Magnitude() > maxAngularVelocity) {
			angularVelocity.Normalize();
			angularVelocity.Mult(maxAngularVelocity);
		}
		Utilities::Vector3d angularChange = angularAcceleration * 0.5 * pow(deltaTime, 2) + angularVelocity * deltaTime; // s = 1/2*a*t^2 + v_0*t + s_0
		gameObject->rotation.pitch += angularChange.X;
		gameObject->rotation.yaw += angularChange.Y;
		gameObject->rotation.roll += angularChange.Z;
		angularVelocity += angularAcceleration * deltaTime; // v = a * t + v_0
		//Reset acceleration
		ResetAcceleration();
	}
	//Constructor
	Physics::Physics() {

	}
	//Force functions
	void Physics::AddForce(Utilities::Vector3d force) {
		force.Divide(mass);
		acceleration += force;
	}
	void Physics::AddForceAtPoint(Utilities::Vector3d force, Utilities::Vector3d point) {

	}
	void Physics::ResetAcceleration() {
		acceleration = Utilities::Vector3d(0, 0, 0);
		angularAcceleration = Utilities::Vector3d(0, 0, 0);
	}
	//Other functions
	void Physics::UpdateColliders() {
		colliders.clear();
		std::vector<Collider*> c = gameObject->GetAllComponents<Collider>();
		colliders.insert(colliders.end(), c.begin(), c.end());
		//Go through children's colliders
		for (int i = 0; i < gameObject->GetChildCount(); i++) {
			GameObject* child = gameObject->GetChild(i);
			std::vector<Collider*> cc = child->GetAllComponents<Collider>();
			colliders.insert(colliders.end(), cc.begin(), cc.end());
		}
		lastColliderUpdate = clock();
	}
	void Physics::UpdateCollisions() {

	}
	void Physics::Gravity() {
		// Formula: F_t = m * g
		if (isAffectedByGravity) {
			Utilities::Vector3d gravityForce = gravityDirection.Get();
			gravityForce.Mult(gravityAcceleration);
			gravityForce.Mult(mass);
			///Log::Normal(std::format("Gravity force: ({};{};{})", gravityForce.X, gravityForce.Y, gravityForce.Z));
			AddForce(gravityForce);
		}
	}
	void Physics::Drag() {
		// Formula: F_d = 0.5 * FluidDensity * (Velocity ^ 2) * Coefficient * Area
		Utilities::Vector3d dragForce = velocity.Get().Mult(-1); // Opposite direction to velocity
		dragForce.Normalize();
		// Area
		double area = 1;
		dragForce.Mult(area);
		dragForce.Mult(0.5); // 0.5
		dragForce.Mult(1.293); // Fluid Density
		dragForce.Mult(pow(velocity.Magnitude(), 2)); //(Velocity ^ 2)
		dragForce.Mult(dragConstant); // Coefficient
		///Log::Normal(std::format("Drag force: ({};{};{})", dragForce.X, dragForce.Y, dragForce.Z));
		//Apply
		AddForce(dragForce);
	}

	//Constraints
	void Constraints::FreezePosition() {
		bool FreezePositionX = true;
		bool FreezePositionY = true;
		bool FreezePositionZ = true;
	}
	void Constraints::FreezeRotation() {
		bool FreezeRotationX = true;
		bool FreezeRotationY = true;
		bool FreezeRotationZ = true;
	}
}