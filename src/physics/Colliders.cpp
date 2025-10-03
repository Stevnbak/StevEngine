#ifdef StevEngine_PHYSICS
#include "Colliders.hpp"
#include "main/GameObject.hpp"
#include "main/Component.hpp"
#include "utilities/Model.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Stream.hpp"
#include "utilities/Terrain.hpp"
#include "utilities/Vector3.hpp"

#include <algorithm>
#include <sstream>
#include <iomanip>

#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include "Jolt/Physics/Collision/Shape/HeightFieldShape.h"
#include <Jolt/Geometry/Triangle.h>
#include <Jolt/Geometry/IndexedTriangle.h>
#include <Jolt/Core/StreamWrapper.h>
#include <Jolt/ObjectStream/ObjectStream.h>
#include <Jolt/ObjectStream/ObjectStreamTextOut.h>


namespace StevEngine::Physics {
	Collider::Collider(JPH::Ref<JPH::Shape> shape, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
  : rawShape(shape) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	void Collider::Start() {
		GameObject& parent = GetParent();
		//Set correct scale for shape
		Utilities::Vector3 abs = parent.GetWorldScale();
		if(rawShape) this->shape = new JPH::ScaledShape(rawShape, Utilities::Vector3(scale.X * abs.X, scale.Y * abs.Y, scale.Z * abs.Z));
		//Events
		handlers.emplace_back(parent.Subscribe<TransformUpdateEvent>([this] (TransformUpdateEvent e) { this->TransformUpdate(e.position, e.rotation, e.scale);}), TransformUpdateEvent::GetStaticEventType());
	}
	void Collider::Deactivate()	{
		if(shape) shape->Release();
	}
	Collider::~Collider() {
		if(shape) shape->Release();
		if(rawShape) rawShape->Release();
	}
	void Collider::TransformUpdate(bool position, bool rotation, bool scale, bool fromLocal) {
		GameObject* parent = &GetParent();
		//Re scale collider
		if(scale) {
			Utilities::Vector3 abs = parent->GetWorldScale();
			this->shape = new JPH::ScaledShape(rawShape, Utilities::Vector3(this->scale.X * abs.X, this->scale.Y * abs.Y, this->scale.Z * abs.Z));
		} else if(!fromLocal) {
			//Don't tell current parent if it gets the same position or rotation change
			parent = &parent->GetParent();
		}
		//Publish shape updated
		if(parent != nullptr) parent->Publish(ColliderUpdateEvent(this));
	}

	//Update transform
	void Collider::SetScale(Utilities::Vector3 scale) {
		this->scale = scale;
		TransformUpdate(false, false, true, true);
	}
	void Collider::SetRotation(Utilities::Quaternion rotation) {
		this->rotation = rotation;
		TransformUpdate(false, true, false, true);
	}
	void Collider::SetPosition(Utilities::Vector3 position) {
		this->position = position;
		TransformUpdate(true, false, false, true);
	}
	void Collider::SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		this->scale = scale;
		this->rotation = rotation;
		this->position = position;
		TransformUpdate(true, true, true, true);
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
	JPH::Ref<JPH::Shape> ModelToShape(const Utilities::Model& model, bool convex) {
		//Create model shape
		JPH::StaticCompoundShapeSettings shapeSettings = JPH::StaticCompoundShapeSettings();
		if(convex) {
			//Create as ConvexHullShape
			for(Utilities::Mesh mesh : model.GetMeshes()) {
				JPH::Array<JPH::Vec3> vertices;
				vertices.reserve(mesh.indices.size());
				for(int i = 0; i < mesh.indices.size(); i++) {
					const Utilities::Vertex& v = mesh.vertices[mesh.indices[i]];
					vertices.emplace_back(v.position);
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
				vertices.reserve(mesh.vertices.size());
				std::transform(mesh.vertices.begin(), mesh.vertices.end(), vertices.begin(), [](Utilities::Vertex v) { return JPH::Float3(v.position.X, v.position.Y, v.position.Z); });
				JPH::IndexedTriangleList indices;
				indices.reserve(mesh.indices.size() / 3);
				for(int i = 0; i < indices.size(); i++) {
					indices.emplace_back(mesh.indices[i*3+0], mesh.indices[i*3+1], mesh.indices[i*3+2]);
				}
				shapeSettings.AddShape(Utilities::Vector3(), Utilities::Quaternion(1,0,0,0), new JPH::MeshShapeSettings(vertices, indices, JPH::PhysicsMaterialList()));
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
	ModelCollider::ModelCollider(const Utilities::Model& model, bool convex, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
	  : Collider(ModelToShape(model, convex), position, rotation, scale) {}

	//Terrain collider
	JPH::Ref<JPH::Shape> TerrainToShape(const Utilities::TerrainData& data) {
		Utilities::Vector3 offset = Utilities::Vector3(((data.size - 1) / 2.0) * (-data.step), 0, ((data.size - 1) / 2.0) * (-data.step));
		Utilities::Vector3 scale = Utilities::Vector3(data.step, 1, data.step);
		float* fPoints = new float[data.size*data.size];
		for(int i = 0; i < data.size*data.size; i++) fPoints[i] = (float)data.points[i];
		JPH::HeightFieldShapeSettings settings = JPH::HeightFieldShapeSettings(fPoints, offset, scale, data.size);
		JPH::ShapeSettings::ShapeResult result = settings.Create();
		if(result.IsValid()) {
			return result.Get();
		}
		else {
			Log::Error(result.GetError().c_str(), true);
			return NULL;
		}
	}
	TerrainCollider::TerrainCollider(const Utilities::TerrainData& data, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
	  : Collider(TerrainToShape(data), position, rotation, scale) {}

	//Export colliders
	Utilities::Stream Collider::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		//Export shape to STL string
		std::stringstream data;
		JPH::StreamOutWrapper stream_out(data);
		JPH::Shape::ShapeToIDMap shape_to_id;
		JPH::Shape::MaterialToIDMap material_to_id;
		rawShape->SaveWithChildren(stream_out, shape_to_id, material_to_id);
		std::string shapeString = data.str();
		stream << (size_t)shapeString.size();
		if(type == Utilities::StreamType::Text) {
			for(size_t i = 0; i < shapeString.size(); i++) {
				stream.GetStream() << std::setw(4) << std::setfill('0') << std::hex << (short)shapeString[i];
			}
		}
		else {
			stream.GetStream() << data.rdbuf();
		}
		//Transform
		stream << position << rotation << scale;

		return stream;
	}
	JPH::Ref<JPH::Shape> ImportShape(Utilities::Stream& stream) {
		//Size of data
		size_t size = stream.Read<size_t>();
		std::stringstream data;
		if(stream.type == Utilities::StreamType::Text) {
			for(size_t c = 0; c < size; c++) {
				char sz[5];
				sz[4] = '\000';
				stream >> sz[0] >> sz[1] >> sz[2] >> sz[3];
				char out = strtoul(sz, NULL, 16);
				data << out;
			}
		} else {
			for(size_t i = 0; i < size; i++) {
				data << stream.Read<char>();
			}
		}

		JPH::StreamInWrapper stream_in = JPH::StreamInWrapper(data);
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
	Collider::Collider(Utilities::Stream& stream) : rawShape(ImportShape(stream)) {
		stream >> position >> rotation >> scale;
	}
}
#endif
