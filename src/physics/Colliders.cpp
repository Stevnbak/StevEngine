#include "Colliders.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Core/StreamWrapper.h>
#include <Jolt/ObjectStream/ObjectStream.h>
#include <Jolt/ObjectStream/ObjectStreamTextOut.h>


namespace StevEngine::Physics {
	Collider::Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
	: rawShape(shape), Component("Collider") {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	void Collider::Start() {
		//Set correct scale for shape
		Utilities::Vector3d abs = this->gameObject->absScale();
		this->shape = new JPH::ScaledShape(rawShape, Utilities::Vector3d(scale.X * abs.X, scale.Y * abs.Y, scale.Z * abs.Z));
	}
	void Collider::Destroy() {
		delete shape;
		delete rawShape;
		delete this;
	}
	void Collider::Draw() {}

	//Cube collider
	CubeCollider::CubeCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::BoxShape(Utilities::Vector3d(0.5, 0.5, 0.5)), position, rotation, scale) {}

	//Sphere collider
	SphereCollider::SphereCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::SphereShape(0.5), position, rotation, scale) {}

	//Cylinder collider
	CylinderCollider::CylinderCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::CylinderShape(0.5,0.5), position, rotation, scale) {}

	//Capsule collider
	CapsuleCollider::CapsuleCollider(Utilities::Vector3d position, Utilities::Rotation3d rotation, Utilities::Vector3d scale) 
		: Collider(new JPH::CapsuleShape(0.5,0.5), position, rotation, scale) {}

	//Export colliders
	void Collider::Export(tinyxml2::XMLElement* element) {
        element->SetAttribute("position", ((std::string)position).c_str());
		element->SetAttribute("rotation", ((std::string)rotation).c_str());
		element->SetAttribute("scale", ((std::string)scale).c_str());
		
		//Export shape to STL string
		std::stringstream data;
		JPH::StreamOutWrapper stream_out(data);
		JPH::Shape::ShapeToIDMap shape_to_id;
		JPH::Shape::MaterialToIDMap material_to_id;
		rawShape->SaveWithChildren(stream_out, shape_to_id, material_to_id);
		std::string shapeString = data.str();
		std::vector<char> vec(shapeString.begin(), shapeString.end());
		std::replace(vec.begin(), vec.end(), '\000', '0');
		element->SetAttribute("shape", std::string(vec.begin(), vec.end()).c_str());
    }
	JPH::Ref<JPH::Shape> ImportShape(tinyxml2::XMLElement* node) {
		std::string raw = node->Attribute("shape");
		std::vector<char> vec(raw.begin(), raw.end());
		std::replace(vec.begin(), vec.end(), '0', '\000');
		std::stringstream data(std::string(vec.begin(), vec.end()));
		JPH::StreamInWrapper stream_in(data);
		JPH::Shape::IDToShapeMap id_to_shape;
		JPH::Shape::IDToMaterialMap id_to_material;
		JPH::Shape::ShapeResult result = JPH::Shape::sRestoreWithChildren(stream_in, id_to_shape, id_to_material);
		if (result.IsValid())
			return result.Get();
		else {
			return NULL;
		}
	}
	Collider::Collider(tinyxml2::XMLElement* node) : rawShape(ImportShape(node)), Component(node) {
		position = Utilities::Vector3d(node->Attribute("position"));
		rotation = Utilities::Rotation3d(node->Attribute("rotation"));
		scale = Utilities::Vector3d(node->Attribute("scale"));
	}
	FactoryBase* colfactory = GameObject::AddComponentFactory<Collider>(std::string("Collider"));
}