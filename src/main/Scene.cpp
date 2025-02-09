#include "Scene.hpp"
#include "main/Component.hpp"
#include "utilities/ID.hpp"
#include "utilities/Serializable.hpp"

using namespace StevEngine::Utilities;

namespace StevEngine {
	ID Scene::CreateObject() {
		ID id;
		gameObjects.insert({id, GameObject(id, "GameObject", name)});
		if(active) GetObject(id)->Start();
		return id;
	}
	ID Scene::CreateObject(std::string name, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		ID id;
		gameObjects.emplace(id, GameObject(id, name, this->name));
		GameObject* object = GetObject(id);
		object->position = position;
		object->rotation = rotation;
		object->scale = scale;
		if(active) object->Start();
		return id;
	}
	Utilities::ID Scene::CreateObject(Resources::Resource file, StreamType type) {
		Stream stream(type);
		stream.ReadFromFile(file);
		return CreateObject(stream);
	}
	Utilities::ID Scene::CreateObject(Stream& stream) {
		Utilities::ID id;
		stream >> id;
		gameObjects.emplace(id, GameObject(id, "GameObject", name));
		GameObject* object = GetObject(id);
		object->Import(stream);
		if(active) object->Start();
		return id;
	}
	GameObject* Scene::GetObject(Utilities::ID id) {
		if(!gameObjects.contains(id)) return nullptr;
		return &gameObjects.at(id);
	}
	std::vector<ID> Scene::GetAllObjects() {
		std::vector<ID> keys;
		keys.reserve(gameObjects.size());
		for(auto&[id, object] : gameObjects) {
			keys.emplace_back(id);
		}
		return keys;
	}
	std::vector<ID> Scene::GetAllParentObjects() {
		std::vector<ID> keys;
		keys.reserve(gameObjects.size());
		for(auto&[id, object] : gameObjects) {
			if(object.parent.IsNull()) keys.emplace_back(id);
		}
		return keys;
	}
	void Scene::DestroyObject(ID id) {
		if(gameObjects.contains(id)) gameObjects.erase(id);
	}
	Scene::Scene(std::string name) : name(name) {
		//Create main camera
		#ifdef StevEngine_SHOW_WINDOW
		activeCamera = GetObject(CreateObject("Main Camera"))->AddComponent(new Visuals::Camera());
		#endif
	}
	Scene::Scene(std::string name, Stream& stream) : name(name) {
		//Create objects
		uint objects = stream.Read<uint>();
		for(uint i = 0; i < objects; i++) {
			CreateObject(stream);
		}
		//Set camera
		#ifdef StevEngine_SHOW_WINDOW
		activeCamera = GetObject(stream.Read<ID>())->GetComponent<Visuals::Camera>();
		#endif
	}
	Stream Scene::Export(StreamType type) {
		Stream stream(type);
		stream << name << (uint)gameObjects.size();
		for(const auto& [id, obj] : gameObjects) {
			stream << obj.Export(type);
		}
		return stream;
	}
	void Scene::Activate() {
		active = true;
		for(auto&[id, object] : gameObjects) {
			object.Start();
		}
	}
	void Scene::Deactivate() {
		active = false;
		for(auto&[id, object] : gameObjects) {
			object.Deactivate();
		}
	}
}
