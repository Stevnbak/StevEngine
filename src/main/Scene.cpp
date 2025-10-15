#include "Scene.hpp"
#include "main/Component.hpp"
#include "main/Log.hpp"
#include "utilities/ID.hpp"
#include "utilities/Stream.hpp"
#include <cassert>

using namespace StevEngine::Utilities;

namespace StevEngine {
	ID Scene::CreateObject() {
		ID id;
		gameObjects.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, "GameObject", name)) ;
		if(active) GetObject(id).Start();
		return id;
	}
	ID Scene::CreateObject(std::string name, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		ID id;
		gameObjects.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, name, this->name)) ;
		GameObject& object = GetObject(id);
		object.position = position;
		object.rotation = rotation;
		object.scale = scale;
		if(active) object.Start();
		return id;
	}
	Utilities::ID Scene::CreateObject(Resources::Resource file, Utilities::StreamType type) {
		Utilities::Stream stream(type);
		stream.ReadFromFile(file);
		return CreateObject(stream);
	}
	Utilities::ID Scene::CreateObject(Utilities::Stream& stream) {
		Utilities::ID id;
		stream >> id;
		if(gameObjects.contains(id)) {
			Log::Error(std::format("GameObject with id \"{}\" already exists.", id.GetString()));
			return id;
		}
		gameObjects.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, "GameObject", name)) ;
		GameObject& object = GetObject(id);
		object.Import(stream);
		if(active) object.Start();
		return id;
	}
	bool Scene::Exists(Utilities::ID id) {
		return gameObjects.contains(id);
	}
	GameObject& Scene::GetObject(Utilities::ID id) {
		assert(gameObjects.contains(id) && "No GameObject found.");
		return gameObjects.at(id);
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
		destroyedObjects.insert(id);
	}
	Scene::Scene(std::string name) : name(name) {
		//Create main camera
		#ifdef StevEngine_SHOW_WINDOW
		activeCamera = GetObject(CreateObject("Main Camera")).AddComponent(new Visuals::Camera());
		#endif
	}
	Scene::Scene(std::string name, Utilities::Stream& stream) : name(name) {
		//Create objects
		uint32_t objects = stream.Read<uint32_t>();
		for(uint32_t i = 0; i < objects; i++) {
			CreateObject(stream);
		}
		//Set camera
		#ifdef StevEngine_SHOW_WINDOW
		activeCamera = GetObject(stream.Read<ID>()).GetComponent<Visuals::Camera>();
		#endif
	}
	Utilities::Stream Scene::Export(Utilities::StreamType type) {
		Utilities::Stream stream(type);
		stream << name << (uint32_t)gameObjects.size();
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
	Scene::~Scene() {
		for(Utilities::ID id : GetAllParentObjects()) {
			GetObject(id).Deactivate();
			gameObjects.erase(id);
		}
	}
}
