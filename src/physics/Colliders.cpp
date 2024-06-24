#include "Colliders.hpp"
#include "RigidBody.hpp"
#include <scenes/GameObject.hpp>
#include "scenes/Scene.hpp"

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
	Collider::Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
	: rawShape(shape), Component("Collider") {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	void Collider::Start() {
		//Set correct scale for shape
		Utilities::Vector3 abs = GetParent()->GetWorldScale();
		if(rawShape) this->shape = new JPH::ScaledShape(rawShape, Utilities::Vector3(scale.X * abs.X, scale.Y * abs.Y, scale.Z * abs.Z));
	}
	void Collider::Deactivate()	{
		if(shape) shape->Release();
	}
	Collider::~Collider() {
		if(shape) shape->Release();
		if(rawShape) rawShape->Release();
	}
	void Collider::Draw() {
		
	}
	void Collider::TransformUpdate(bool position, bool rotation, bool scale) {
		GameObject* latest = GetParent();
		//Re scale collider
		if(scale) {
			Utilities::Vector3 abs = latest->GetWorldScale();
			this->shape = new JPH::ScaledShape(rawShape, Utilities::Vector3(this->scale.X * abs.X, this->scale.Y * abs.Y, this->scale.Z * abs.Z));
		}
		//Tell all related rigidbodies to update shape
		while(latest != 0) {
			RigidBody* rb = latest->GetComponent<RigidBody>(false);
			if(rb != nullptr) {
				rb->RefreshShape();
			}
			latest = latest->GetParent();
		}
	}

	//Update transform
	void Collider::SetScale(Utilities::Vector3 scale) {
		this->scale = scale;
		TransformUpdate(false, false, true);
	}
	void Collider::SetRotation(Utilities::Quaternion rotation) {
		this->rotation = rotation;
		TransformUpdate(false, true, false);
	}
	void Collider::SetPosition(Utilities::Vector3 position) {
		this->position = position;
		TransformUpdate(true, false, false);
	}
	void Collider::SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		this->scale = scale;
		this->rotation = rotation;
		this->position = position;
		TransformUpdate(true, true, true);
	}
	//Cube collider
	CubeCollider::CubeCollider(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
		: Collider(new JPH::BoxShape(Utilities::Vector3(0.5, 0.5, 0.5)), position, rotation, scale) {}

	//Sphere collider
	SphereCollider::SphereCollider(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
		: Collider(new JPH::SphereShape(0.5), position, rotation, scale) {}

	//Cylinder collider
	CylinderCollider::CylinderCollider(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
		: Collider(new JPH::CylinderShape(0.5,0.5), position, rotation, scale) {}

	//Capsule collider
	CapsuleCollider::CapsuleCollider(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
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
		std::stringstream out;
		for(size_t i = 0; i < shapeString.size(); i++) out << std::setw(4) << std::setfill('0') << std::hex << (short)shapeString[i];
		std::string test = out.str();
		element->SetAttribute("shape", out.str().c_str());
    }
	JPH::Ref<JPH::Shape> ImportShape(tinyxml2::XMLElement* node) {
		std::string raw = node->Attribute("shape");
		std::stringstream data;
		for (size_t i = 0; i < raw.length(); i += 4)
		{
			char sz[5] = {raw[i], raw[i+1], raw[i+2], raw[i+3], '\0'};
			char out = strtoul(sz, NULL, 16);
			data << out;
		}
		std::string test = data.str();
		JPH::StreamInWrapper stream_in(data);
		JPH::Shape::IDToShapeMap id_to_shape;
		JPH::Shape::IDToMaterialMap id_to_material;
		JPH::Shape::ShapeResult result = JPH::Shape::sRestoreWithChildren(stream_in, id_to_shape, id_to_material);
		if (result.IsValid())
			return result.Get();
		else {
			Log::Error(std::format("Collider failed to import shape. Error: {}", result.GetError()));
			return NULL;
		}
	}
	Collider::Collider(tinyxml2::XMLElement* node) : rawShape(ImportShape(node)), Component(node) {
		position = Utilities::Vector3(node->Attribute("position"));
		rotation = Utilities::Quaternion(node->Attribute("rotation"));
		scale = Utilities::Vector3(node->Attribute("scale"));
	}
	FactoryBase* colfactory = GameObject::AddComponentFactory<Collider>(std::string("Collider"));
}