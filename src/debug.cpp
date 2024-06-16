#include <core/Engine.hpp>
#include <core/GameObject.hpp>
#include <visuals/BasicComponents.hpp>
#include <physics/RigidBody.hpp>
#include <physics/Colliders.hpp>
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
		void Export(tinyxml2::XMLElement* element);
		CameraController() : Component("CameraController") {};
		CameraController(tinyxml2::XMLElement* element) : Component("CameraController") {
			movementSpeed = element->DoubleAttribute("movementSpeed");
			sensitivity = element->DoubleAttribute("sensitivity");
		};
};
//FactoryBase* factory = GameObject::AddComponentFactory<CameraController>(std::string("CameraController"));

void CameraController::Export(tinyxml2::XMLElement* element) {
	element->SetAttribute("movementSpeed", movementSpeed);
	element->SetAttribute("sensitivity", sensitivity);
}
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
	Engine::Instance->activeCamera->gameObject->rotation.roll = 0;
}

int main(int argc, char** argv) {
	//Create engine
	Engine engine = Engine("StevnGame", 100, false, mainUpdate);
	//Setup debug stuff:
	Log::Normal("Start");
	glClearColor(0, 0, 0, 1);
	//Create test objects
	{
		GameObject* floor = GameObject::Create("Cube", Utilities::Vector3d(0, -1, 0), Utilities::Rotation3d(0,0,15), Utilities::Vector3d(100, 1, 100));
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
		JPH::AABox bounds = collider->shape->GetLocalBounds();
		Utilities::Vector3d center = collider->shape->GetCenterOfMass();
		std::string test = cube->Export();
		cube->ExportToFile("cube.xml");
		Log::Normal(std::string("XML Cube Export: ") + test);
		GameObject* cube2 = GameObject::CreateFromFile(Engine::Instance->resources->GetFile("cube.xml"));
		Physics::Collider* col = cube2->GetComponent<Physics::Collider>();
		JPH::AABox bounds2 = col->shape->GetLocalBounds();
		Utilities::Vector3d center2 = col->shape->GetCenterOfMass();
		Primitive* primitive2 = cube2->GetComponent<Primitive>();
		primitive2->colour = SDL_Color(1, 1, 1, 1);
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
		///sphere->Destroy();
	}
	//Add Camera controller
	///Engine::Instance->activeCamera->gameObject->AddComponent(new CameraController());
	Engine::Instance->activeCamera->gameObject->position = Utilities::Vector3d(0, 4, 15);
	Engine::Instance->activeCamera->gameObject->rotation = Utilities::Rotation3d(0, 0, 0);

	//Test ressource manager
	Log::Normal(std::format("Ressource path: {}", Engine::Instance->resources->resourcePath));
	Log::Normal(std::format("Ressource 0: {}", Engine::Instance->resources->GetFile(0).path));
	Log::Normal(std::format("Ressource \"test.txt\": {}", Resources::DataToText(Engine::Instance->resources->GetFile("test.txt").GetData())));
	Log::Normal(std::format("Ressource \"test.txt\": {}", Resources::DataToText(Engine::Instance->resources->GetFile("test.txt").GetData())));

	//Play audio
	GameObject* audioPlayer = GameObject::Create("Audio Player");
	Audio::Emitter* emitter = audioPlayer->AddComponent(new Audio::Emitter("audio.wav", false));
	///Audio::Emitter emitter2 = Audio::Emitter("audio.wav", true);
	emitter->Play();
	///emitter2.Play();

	//Export gameobject
	GameObject* object = GameObject::Create("Test object", Utilities::Vector3d(2, 3, 4));
	object->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving"), 2));
	std::string test = object->Export();
	Log::Normal(std::string("XML Export: ") + test);
	const char* t = test.c_str();

	GameObject* object2 = GameObject::CreateFromFile(Engine::Instance->resources->GetFile("test.xml"));

	//Start engine
	engine.Start();
}