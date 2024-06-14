#include "Colliders.hpp"

#include <stdexcept>
#include <algorithm>

#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>


namespace StevEngine::Physics {
	Collider::Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale, std::string type) 
		: Component(type) {
		this->shape = shape;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	void Collider::Start() {
		//Set correct scale for shape
		Utilities::Vector3d abs = this->gameObject->absScale();
		this->shape = new JPH::ScaledShape(shape, Utilities::Vector3d(scale.X * abs.X, scale.Y * abs.Y, scale.Z * abs.Z));
	}
	void Collider::Destroy() {
		delete shape;
		delete this;
	}
	void Collider::Draw() {}

	//Cube collider
	CubeCollider::CubeCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::BoxShape(Utilities::Vector3d(0.5, 0.5, 0.5)), position, rotation, scale, "CubeCollider") {}

	//Sphere collider
	SphereCollider::SphereCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::SphereShape(0.5), position, rotation, scale, "SphereCollider") {}

	//Cylinder collider
	CylinderCollider::CylinderCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::CylinderShape(0.5,0.5), position, rotation, scale, "CylinderCollider") {}

	//Capsule collider
	CapsuleCollider::CapsuleCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::CapsuleShape(0.5,0.5), position, rotation, scale, "CapsuleCollider") {}

	//Export colliders
	void Collider::Export(tinyxml2::XMLElement* element) {
        
    }
	Collider::Collider(tinyxml2::XMLElement* node) : Component(node) {}
}