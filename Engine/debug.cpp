#include <Core/Engine.hpp>
#include <Visuals/BasicComponents.hpp>
#include <Physics/RigidBody.hpp>
#include <Physics/Layers.hpp>
#include <Core/ResourceManager.hpp>

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


void mainStart() {
	Log::Normal("Main start");
	glClearColor(0, 0, 0, 1);
	//Create test objects
	{
		GameObject* floor = GameObject::Create();
		floor->scale = Utilities::Vector3d(10, 1, 10);
		floor->position = Utilities::Vector3d(0, -1, 0);
		Primitive* primitive = floor->AddComponent(new Primitive());
		primitive->type = PrimitiveType::Cube;
		primitive->colour = SDL_Color(0, 1, 0, 1);
		Physics::CubeCollider* collider = floor->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* pb = floor->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
	}
	{
		GameObject* cube = GameObject::Create();
		cube->scale = Utilities::Vector3d(1, 1, 1);
		cube->position = Utilities::Vector3d(0, 2, 0);
		Primitive* primitive = cube->AddComponent(new Primitive());
		primitive->type = PrimitiveType::Cube;
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::CubeCollider* collider = cube->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* pb = cube->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	/**GameObject* testSphere = GameObject::Create();
	testSphere->scale = Utilities::Vector3d(3, 3, 3); 
	testSphere->position = Utilities::Vector3d(2, 1, -1);
	Primitive* sphere = testSphere->AddComponent<Primitive>();
	sphere->type = PrimitiveType::Sphere;
	sphere->colour = SDL_Color(1, 0, 0, 1);
	GameObject* testCylinder = GameObject::Create(); 
	testCylinder->scale = Utilities::Vector3d(1, 1, 3); 
	testCylinder->position = Utilities::Vector3d(1, 2, 3);
	testCylinder->rotation = Utilities::Rotation3d(45,0,0);
	Primitive* cylinder = testCylinder->AddComponent<Primitive>();
	cylinder->type = PrimitiveType::Cylinder;
	cylinder->colour = SDL_Color(0, 0, 1, 1);
	GameObject* testCapsule = GameObject::Create();
	testCapsule->scale = Utilities::Vector3d(1, 1, 2);
	testCapsule->position = Utilities::Vector3d(-4, -2, 1);
	testCapsule->rotation = Utilities::Rotation3d(-90, 0, 0);
	Primitive* capsule = testCapsule->AddComponent<Primitive>();
	capsule->type = PrimitiveType::Capsule;
	capsule->colour = SDL_Color(0, 1, 1, 1);
	GameObject* testQuad = GameObject::Create(); 
	testQuad->scale = Utilities::Vector3d(2, 2, 1);
	testQuad->position = Utilities::Vector3d(-5, 0, 5);
	Primitive* quad = testQuad->AddComponent<Primitive>();
	quad->type = PrimitiveType::Quad;
	quad->colour = SDL_Color(1, 1, 0, 1);*/

	//Add Camera controller
	///ActiveCamera->gameObject->AddComponent(new CameraController());
	ActiveCamera->gameObject->position = Utilities::Vector3d(0, 5, 10);
	ActiveCamera->gameObject->rotation = Utilities::Rotation3d(0, 0, 0);

	//Resource manager test
	///const ResourceManager::Resource* res = ResourceManager::GetResource(4);

	//End
	Log::Normal("Main start end");
}

int main(int argc, char** argv) {
	return StartEngine("StevnGame", false, mainUpdate, mainStart);
}