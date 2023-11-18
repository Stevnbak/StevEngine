#include "Physics.hpp"

namespace StevEngine::Physics {
	//Basic functions
	void Physics::Start() {
		UpdateColliders();
	}
	void Physics::Update(double deltaTime) {

	}
	void Physics::Draw() {

	}
	//Constructor
	Physics::Physics() {

	}
	//Force functions
	void Physics::AddImpulseForce(Utilities::Vector3d force) {

	}
	void Physics::AddContinuousForce(Utilities::Vector3d force) {

	}
	void Physics::AddForceAtPoint(Utilities::Vector3d force, Utilities::Vector3d point) {

	}
	void Physics::ResetAllForces() {

	}
	//Other functions
	void Physics::UpdateColliders() {

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