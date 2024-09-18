#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Log.hpp"
#include "physics/Colliders.hpp"
#include "physics/RigidBody.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneManager.hpp"
#include "utilities/Random.hpp"
#include "visuals/Camera.hpp"
#include "visuals/Primitive.hpp"
#include "visuals/render/Lights.hpp"
#include <cstdlib>
#include <string>

using namespace StevEngine;
using namespace Utilities;

void CreateRenderWorld(Scene* scene) {

}
const static int PHYSICS_OBJECTS = 2000;
const static int AREA_SIZE = 200;

void SpawnPhysicsObjects(Scene* scene) {
	GameObject* object = scene->GetObject(scene->CreateObject("Floor", Vector3(0), Quaternion(), Vector3(AREA_SIZE, 1, AREA_SIZE)));
	Visuals::CubePrimitive* primitive = object->AddComponent(new Visuals::CubePrimitive());
	primitive->SetColor(Color(0, 0, 255, 255 ));
	object->AddComponent(new Physics::CubeCollider());
	object->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
	for(int i = 0; i < PHYSICS_OBJECTS; i++) {
		GameObject* object = scene->GetObject(scene->CreateObject("Cube" + std::to_string(i), Vector3(GetRandomDouble(-AREA_SIZE / 2.0, AREA_SIZE / 2.0), 10, GetRandomDouble(-AREA_SIZE / 2.0, AREA_SIZE / 2.0)), Quaternion(), Vector3(1, 1, 1)));
		Visuals::CubePrimitive* primitive = object->AddComponent(new Visuals::CubePrimitive());
		primitive->SetColor(Color(255, 255, 255, 255 ));
		object->AddComponent(new Physics::CubeCollider());
		object->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	//Lights
	for(int i = 0; i < AREA_SIZE; i++) {
		GameObject* object = scene->GetObject(scene->CreateObject("Light" + std::to_string(i), Vector3(GetRandomDouble(-AREA_SIZE / 2.0, AREA_SIZE / 2.0), 15, GetRandomDouble(-AREA_SIZE / 2.0, AREA_SIZE / 2.0)), Quaternion(), Vector3(1, 1, 1)));
		object->AddComponent(new Render::PointLight(Vector3(GetRandomDouble(1), 0, GetRandomDouble(1)), Vector3(GetRandomDouble(0.25), GetRandomDouble(0.25), GetRandomDouble(0.25))));
	}
}

int main(int argc, char** argv) {
	CreateEngine("Performance", { .vsync = false, .fullscreen = false, .WIDTH = 1920, .HEIGHT = 1080, .targetFPS = -1 });
	engine->GetEvents()->Subscribe<UpdateEvent>([](UpdateEvent) { Log::Debug("FPS: " + std::to_string(engine->getFPS())); });

	Scene* scene = sceneManager.CreateScene("Physics test");
	SpawnPhysicsObjects(scene);
	GameObject* camObj = scene->GetCameraObject();
	Visuals::Camera* cam = scene->GetCamera();
	cam->farClip = 1000;
	camObj->SetPosition(Vector3(0, 10, 100));
	camObj->SetRotation(Quaternion::FromAngleAxis(Quaternion::DegreesToRadians(-45), Vector3::right));

	return engine->Start();
}

int wmain(int argc, char** argv) {
	return main(argc, argv);
}
