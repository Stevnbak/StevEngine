#include "PhysicsBody.hpp"
#include "Physics.hpp"
#include <math.h>
#include <iostream>
#include "Log.hpp"
#include "Engine.hpp"

namespace StevEngine::Physics {
	//Constructor
	PhysicsBody::PhysicsBody(JPH::EMotionType motionType, JPH::ObjectLayer layer) {
		this->motionType = motionType;
		this->layer = layer;
	}
	//Basic functions
	void PhysicsBody::Start() {
		//Get collider info
		UpdateColliders();
		//Create body
		///delete body;
		JPH::BodyCreationSettings bodySettings = JPH::BodyCreationSettings(shape, gameObject->position, gameObject->rotation, motionType, layer);
		bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::MassAndInertiaProvided;
		bodySettings.mMassPropertiesOverride.mMass = 1;
		body = physics->bodyInterface->CreateBody(bodySettings);
		physics->bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
	}
	void PhysicsBody::Update(double deltaTime) {
		Log::Normal(std::format("Position: {},{},{}", body->GetPosition().GetX(), body->GetPosition().GetY(), body->GetPosition().GetZ()));
		this->gameObject->position = body->GetPosition();
	}
	void PhysicsBody::Destroy() {
		///delete this->shape;
		///delete this->body;
		delete this;
	}
	//Force functions
	void PhysicsBody::AddForce(Utilities::Vector3d force) {
		force.Divide(mass);
		acceleration += force;
	}
	void PhysicsBody::AddImpulseForce(Utilities::Vector3d force, double time) {
		activeImpulseForces.push_back(std::pair<Utilities::Vector3d, double>(force, time));
	}
	void PhysicsBody::AddForceAtPoint(Utilities::Vector3d force, Utilities::Vector3d point) {

	}
	void PhysicsBody::ResetAcceleration() {
		acceleration = Utilities::Vector3d(0, 0, 0);
		angularAcceleration = Utilities::Vector3d(0, 0, 0);
	}
	//Other functions
	void PhysicsBody::UpdateImpulseForces(double deltaTime) {
		for (int i = 0; i < activeImpulseForces.size(); i++) {
			std::pair<Utilities::Vector3d, double>* value = &activeImpulseForces[i];
			value->second -= deltaTime;
			if (value->second <= 0) {
				value->first = value->first * ((deltaTime + value->second) / deltaTime);
				activeImpulseForces.erase(activeImpulseForces.begin() + i);
			}
			AddForce(value->first);
		}
	}
	void PhysicsBody::UpdateColliders() {
		//Find all colliders:
		std::vector<Collider*> c = gameObject->GetAllComponents<Collider>();
		for (int i = 0; i < gameObject->GetChildCount(); i++) {
			GameObject* child = gameObject->GetChild(i);
			std::vector<Collider*> cc = child->GetAllComponents<Collider>();
			c.insert(c.end(), cc.begin(), cc.end());
		}
		//Clear old info
		colliders.clear();
		///delete shape;
		JPH::MutableCompoundShapeSettings shapeSettings = JPH::MutableCompoundShapeSettings();
		//Add new
		for(Collider* col : c) {
			if(std::find(colliders.begin(), colliders.end(), col) == colliders.end()) {
				colliders.push_back(col);
				shapeSettings.AddShape((col->gameObject->absPosition() + col->getPosition()), (col->gameObject->absRotation() + col->getRotation()), col->shape);
			}
		}
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid())
			shape = result.Get();
		else
			Log::Error(result.GetError().c_str());
	}
	void PhysicsBody::UpdateCollisions() {

	}
	void PhysicsBody::Gravity() {
		// Formula: F_t = m * g
		if (isAffectedByGravity) {
			Utilities::Vector3d gravityForce = gravityDirection.Get();
			gravityForce.Mult(gravityAcceleration);
			gravityForce.Mult(mass);
			///Log::Normal(std::format("Gravity force: ({};{};{})", gravityForce.X, gravityForce.Y, gravityForce.Z));
			AddForce(gravityForce);
		}
	}
	void PhysicsBody::Drag() {
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