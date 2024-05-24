#include "Colliders.hpp"

#include <stdexcept>
#include <algorithm>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>


namespace StevEngine::Physics {
	Collider::Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) {
		this->shape = shape;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	void Collider::Start() {
		//Set correct scale for shape
		this->shape = new JPH::ScaledShape(shape, scale + this->gameObject->absScale());
	}
	void Collider::Destroy() {
		delete shape;
		delete this;
	}
	//Cube collider
	CubeCollider::CubeCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) : Collider(new JPH::BoxShape(Utilities::Vector3d(1,1,1)), position, rotation, scale) {}
}