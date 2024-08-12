#ifdef StevEngine_PHYSICS
#include "Colliders.hpp"
#include "RigidBody.hpp"
#include <scenes/GameObject.hpp>
#include "scenes/Scene.hpp"
#include "main/Engine.hpp"
#include "main/ResourceManager.hpp"
#include "utilities/Model.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include "Jolt/Physics/Collision/Shape/ConvexHullShape.h"
#include "Jolt/Physics/Collision/Shape/StaticCompoundShape.h"
#include "Jolt/Geometry/Triangle.h"
#include "Jolt/Geometry/IndexedTriangle.h"
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
	void Collider::Draw(glm::mat4x4 transform) {

	}
	void Collider::TransformUpdate(bool position, bool rotation, bool scale) {
		GameObject* latest = GetParent();
		//Re scale collider
		if(scale) {
			Utilities::Vector3 abs = latest->GetWorldScale();
			this->shape = new JPH::ScaledShape(rawShape, Utilities::Vector3(this->scale.X * abs.X, this->scale.Y * abs.Y, this->scale.Z * abs.Z));
		} else {
			//Don't tell current rigidbody if it gets the same position or rotation change
			latest = latest->GetParent();
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
	void Collider::LocalTransformUpdate(bool position, bool rotation, bool scale) {
		TransformUpdate(position, rotation, scale);
		if(!scale) {
			RigidBody* rb = GetParent()->GetComponent<RigidBody>(false);
			if(rb != nullptr) {
				rb->RefreshShape();
			}
		}
	}

	//Update transform
	void Collider::SetScale(Utilities::Vector3 scale) {
		this->scale = scale;
		LocalTransformUpdate(false, false, true);
	}
	void Collider::SetRotation(Utilities::Quaternion rotation) {
		this->rotation = rotation;
		LocalTransformUpdate(false, true, false);
	}
	void Collider::SetPosition(Utilities::Vector3 position) {
		this->position = position;
		LocalTransformUpdate(true, false, false);
	}
	void Collider::SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		this->scale = scale;
		this->rotation = rotation;
		this->position = position;
		LocalTransformUpdate(true, true, true);
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
		: Collider(new JPH::CapsuleShape(0.25,0.5), position, rotation, scale) {}

	//Model collider
	JPH::Ref<JPH::Shape> ModelToShape(Utilities::Model model, bool convex) {
		//Create model shape
		JPH::StaticCompoundShapeSettings shapeSettings = JPH::StaticCompoundShapeSettings();
		if(convex) {
			//Create as ConvexHullShape
			for(Utilities::Mesh mesh : model.GetMeshes()) {
				JPH::Array<JPH::Vec3> vertices;
				vertices.resize(mesh.indices.size());
				for(int i = 0; i < mesh.indices.size(); i++) {
					Utilities::Vertex v = mesh.vertices[mesh.indices[i]];
					vertices[i] = JPH::Vec3(v.x, v.y, v.z);
				};
				shapeSettings.AddShape(Utilities::Vector3(), Utilities::Quaternion(1,0,0,0), new JPH::ConvexHullShapeSettings(vertices));
				continue;
				JPH::ConvexHullShapeSettings meshShape = JPH::ConvexHullShapeSettings(vertices);
				JPH::ShapeSettings::ShapeResult meshResult = meshShape.Create();
				if(meshResult.IsValid()) {
					shapeSettings.AddShape(JPH::Vec3(), JPH::Quat(), meshResult.Get());
				} else {
					Log::Error(meshResult.GetError().c_str(), true);
				}
			}
		} else {
			//Create as mesh
			for(Utilities::Mesh mesh : model.GetMeshes()) {
				JPH::VertexList vertices;
				vertices.resize(mesh.vertices.size());
				std::transform(mesh.vertices.begin(), mesh.vertices.end(), vertices.begin(), [](Utilities::Vertex v) { return JPH::Float3(v.x, v.y, v.z); });
				JPH::IndexedTriangleList indices;
				indices.resize(mesh.indices.size() / 3);
				for(int i = 0; i < indices.size(); i++) {
					indices[i] = JPH::IndexedTriangle(mesh.indices[i*3+0], mesh.indices[i*3+1], mesh.indices[i*3+2]);
				}
				shapeSettings.AddShape(Utilities::Vector3(), Utilities::Quaternion(1,0,0,0), new JPH::MeshShapeSettings(vertices, indices, JPH::PhysicsMaterialList()));
				continue;
				JPH::MeshShapeSettings meshShape = JPH::MeshShapeSettings(vertices, indices, JPH::PhysicsMaterialList());
				JPH::ShapeSettings::ShapeResult meshResult = meshShape.Create();
				if(meshResult.IsValid()) {
					shapeSettings.AddShape(Utilities::Vector3(), Utilities::Quaternion(1,0,0,0), new JPH::MeshShapeSettings(vertices, indices, JPH::PhysicsMaterialList()));
				} else {
					Log::Error(meshResult.GetError().c_str(), true);
				}
			}
		}
		//Create final shape
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid()) {
			return result.Get();
		}
		else {
			Log::Error(result.GetError().c_str(), true);
			return NULL;
		}
	}
	ModelCollider::ModelCollider(Utilities::Model model, bool convex, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
		: Collider(ModelToShape(model, convex), position, rotation, scale) {}

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
			Log::Error(std::format("Collider failed to import shape. Error: {}", result.GetError()), true);
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
#endif
