#include <core/Engine.hpp>
#include <visuals/BasicComponents.hpp>
#include <physics/RigidBody.hpp>
#include <physics/Layers.hpp>
#include <core/ResourceManager.hpp>

#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

using namespace StevEngine;

class CameraController : public Component {
	public:
		double movementSpeed = 4;
		double sensitivity = 5;
		void Draw() {}
		void Update(double deltaTime);
		void Start();
};
void CameraController::Update(double deltaTime) {
	if (InputSystem::cursorMode == InputSystem::CursorMode::Free) {
		return;
	}
	//Move position
	Utilities::Vector3d forward = gameObject->rotation.forward();
	Utilities::Vector3d right = gameObject->rotation.right();
	Utilities::Vector3d up = gameObject->rotation.up();
	if (InputSystem::IsKeyPressed(SDLK_w)) {
		gameObject->position -= forward * movementSpeed * deltaTime;
	} 
	if (InputSystem::IsKeyPressed(SDLK_s)) {
		gameObject->position += forward * movementSpeed * deltaTime;
	}
	if (InputSystem::IsKeyPressed(SDLK_d)) {
		gameObject->position += right * movementSpeed * deltaTime;
	}
	if (InputSystem::IsKeyPressed(SDLK_a)) {
		gameObject->position -= right * movementSpeed * deltaTime;
	}
	if (InputSystem::IsKeyPressed(SDLK_SPACE)) {
		gameObject->position += up * movementSpeed * deltaTime;
	}
	if (InputSystem::IsKeyPressed(SDLK_LSHIFT)) {
		gameObject->position -= up * movementSpeed * deltaTime;
	}
	//Look around
	gameObject->rotation.yaw -= InputSystem::mouseDelta.X * sensitivity * deltaTime;
	gameObject->rotation.pitch -= InputSystem::mouseDelta.Y * sensitivity * deltaTime;
}
void CameraController::Start() {
	InputSystem::cursorMode = InputSystem::CursorMode::Locked;
	InputSystem::AddKeyDownEvent([](SDL_Keycode key) {
		if (key == SDLK_ESCAPE) {
			if (InputSystem::cursorMode == InputSystem::CursorMode::Free) {
				InputSystem::cursorMode = InputSystem::CursorMode::Locked;
			}
			else {
				InputSystem::cursorMode = InputSystem::CursorMode::Free;
				InputSystem::cursorVisible = true;
			}
		}
	});
}

void mainUpdate(double deltaTime) {
	/*Utilities::Rotation3d testQ(0, 0, 90);
	Utilities::Vector3d forward = testQ.forward();
	Log::Normal(std::format("Forward: ({};{};{})", forward.X, forward.Y, forward.Z));
	Utilities::Vector3d right = testQ.right();
	Log::Normal(std::format("Right: ({};{};{})", right.X, right.Y, right.Z));
	Utilities::Vector3d up = testQ.up();
	Log::Normal(std::format("Up: ({};{};{})", up.X, up.Y, up.Z));
	Log::Normal("----------------------");*/
	ActiveCamera->gameObject->rotation.roll = 0;
}

const ResourceManager::ResourceSystem resourceSystem = ResourceManager::ResourceSystem(std::filesystem::absolute("./assets").generic_string());

void mainStart() {
	Log::Normal("Main start");
	glClearColor(0, 0, 0, 1);
	//Create test objects
	{
		GameObject* floor = GameObject::Create("Cube", Utilities::Vector3d(0, -1, 0), Utilities::Rotation3d(0,0,0), Utilities::Vector3d(10, 1, 10));
		Primitive* primitive = floor->AddComponent(new Primitive(PrimitiveType::Cube));
		primitive->colour = SDL_Color(0, 1, 0, 1);
		Physics::CubeCollider* collider = floor->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = floor->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
	}
	{
		GameObject* cube = GameObject::Create("Cube", Utilities::Vector3d(0, 4, 0), Utilities::Rotation3d(), Utilities::Vector3d(1, 2, 1));
		Primitive* primitive = cube->AddComponent(new Primitive(PrimitiveType::Cube));
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::CubeCollider* collider = cube->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = cube->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	{
		GameObject* sphere = GameObject::Create("Sphere", Utilities::Vector3d(3, 3, 0));
		Primitive* primitive = sphere->AddComponent(new Primitive(PrimitiveType::Sphere));
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::Collider* collider = sphere->AddComponent(new Physics::SphereCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	{
		GameObject* sphere = GameObject::Create("Cylinder", Utilities::Vector3d(0, 3, 3));
		Primitive* primitive = sphere->AddComponent(new Primitive(PrimitiveType::Cylinder));
		primitive->colour = SDL_Color(0, 0, 1, 1);
		Physics::Collider* collider = sphere->AddComponent(new Physics::CylinderCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	//Add Camera controller
	///ActiveCamera->gameObject->AddComponent(new CameraController());
	ActiveCamera->gameObject->position = Utilities::Vector3d(0, 5, 10);
	ActiveCamera->gameObject->rotation = Utilities::Rotation3d(0, 0, 0);

	//Test ressource manager
	Log::Normal(std::format("Ressource path: {}", resourceSystem.resourcePath));
	Log::Normal(std::format("Ressource 0: {}", resourceSystem.GetFile(0).path));
	Log::Normal(std::format("Ressource \"test.txt\": {}", ResourceManager::DataToText(resourceSystem.GetFile("test.txt").data)));

	//End
	Log::Normal("Main start end");
}

int main(int argc, char** argv) {
	//Start engine
	return StartEngine("StevnGame", false, mainUpdate, mainStart);
}