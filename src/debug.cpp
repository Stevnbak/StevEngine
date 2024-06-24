#include <main/Engine.hpp>
#include <scenes/GameObject.hpp>
#include <visuals/BasicComponents.hpp>
#include <physics/RigidBody.hpp>
#include <physics/Colliders.hpp>
#include <physics/Layers.hpp>
#include <main/ResourceManager.hpp>

#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

using namespace StevEngine;
using namespace StevEngine::Utilities;

/**
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
	Utilities::Vector3 forward = gameObject->rotation.Forward();
	Utilities::Vector3 right = gameObject->rotation.Right();
	Utilities::Vector3 up = gameObject->rotation.Up();
	if (InputSystem::IsKeyPressed(SDLK_w)) {
		gameObject->SetPosition(gameObject->GetPosition() - forward * movementSpeed * deltaTime);
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
	gameObject->rotation *= Quaternion::FromAngleAxis(InputSystem::mouseDelta.X * sensitivity * deltaTime, up);
	gameObject->rotation *= Quaternion::FromAngleAxis(InputSystem::mouseDelta.Y * sensitivity * deltaTime, right);
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
 */

void mainUpdate(double deltaTime) {
	/*Utilities::Quaternion testQ(0, 0, 90);
	Utilities::Vector3 forward = testQ.forward();
	Log::Normal(std::format("Forward: ({};{};{})", forward.X, forward.Y, forward.Z));
	Utilities::Vector3 right = testQ.right();
	Log::Normal(std::format("Right: ({};{};{})", right.X, right.Y, right.Z));
	Utilities::Vector3 up = testQ.up();
	Log::Normal(std::format("Up: ({};{};{})", up.X, up.Y, up.Z));
	Log::Normal("----------------------");*/
}

//Debug files
extern const char _binary____assets_test_txt_start[];
extern const char _binary____assets_test_txt_end[];
extern const char _binary____assets_test_2_txt_start[];
extern const char _binary____assets_test_2_txt_end[];
extern const char _binary____assets_audio_wav_start[];
extern const char _binary____assets_audio_wav_end[];
extern const char _binary____assets_cube_object_start[];
extern const char _binary____assets_cube_object_end[];
extern const char _binary____assets_Debug_scene_scene_start[];
extern const char _binary____assets_Debug_scene_scene_end[];

int main(int argc, char** argv) {
	//Create engine
	Engine engine = Engine("StevnGame", 100, false, mainUpdate);
	//Setup debug stuff:
	Log::Normal("Start");
	glClearColor(0, 0, 0, 1);
	
	//Add debug assets
	engine.resources.AddFile("test.txt", &_binary____assets_test_txt_start[0], _binary____assets_test_txt_end - _binary____assets_test_txt_start);
	engine.resources.AddFile("test_2.txt", &_binary____assets_test_2_txt_start[0], _binary____assets_test_2_txt_end - _binary____assets_test_2_txt_start);
	engine.resources.AddFile("audio.wav", &_binary____assets_audio_wav_start[0], _binary____assets_audio_wav_end - _binary____assets_audio_wav_start);
	engine.resources.AddFile("cube.object", &_binary____assets_cube_object_start[0], _binary____assets_cube_object_end - _binary____assets_cube_object_start);
	engine.resources.AddFile("Debug_scene.scene", &_binary____assets_Debug_scene_scene_start[0], _binary____assets_Debug_scene_scene_end - _binary____assets_Debug_scene_scene_start);

	//Create new scene
	//Scene* scene = engine.scenes.CreateSceneFromFile(engine.resources.GetFile("Debug_scene.scene"));
	Scene* scene = engine.scenes.CreateScene("Debug scene");

	//Create test objects
	{
		ID id = scene->CreateObject("Cube", Utilities::Vector3(0, -1, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(15), Utilities::Vector3::forward), Utilities::Vector3(100, 1, 100));
		GameObject* floor = scene->GetObject(id);
		Primitive* primitive = floor->AddComponent(new Primitive(PrimitiveType::Cube));
		primitive->colour = SDL_Color(0, 1, 0, 1);
		Physics::CubeCollider* collider = floor->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = floor->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
	}
	{
		ID id = scene->CreateObject("Cube", Utilities::Vector3(0, 4, 0), Utilities::Quaternion(), Utilities::Vector3(1, 2, 1));
		GameObject* cube = scene->GetObject(id);
		Primitive* primitive = cube->AddComponent(new Primitive(PrimitiveType::Cube));
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::CubeCollider* collider = cube->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = cube->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		int test = (int)JPH::EShapeSubType::Box;
		rb->SetMotionProperties(Physics::MotionProperties(1, 0.5));
		cube->ExportToFile("cube");
		ID id2 = scene->CreateObjectFromFile(Engine::Instance->resources.GetFile("cube.object"));
		GameObject* cube2 = scene->GetObject(id2);
		Physics::Collider* col = cube2->GetComponent<Physics::Collider>();
		Primitive* primitive2 = cube2->GetComponent<Primitive>();
		primitive2->colour = SDL_Color(1, 1, 1, 1);
	}
	{
		ID id = scene->CreateObject("Sphere", Utilities::Vector3(3, 3, 0));
		GameObject* sphere = scene->GetObject(id);
		Primitive* primitive = sphere->AddComponent(new Primitive(PrimitiveType::Sphere));
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::Collider* collider = sphere->AddComponent(new Physics::SphereCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	{
		ID id = scene->CreateObject("Cylinder", Utilities::Vector3(0, 3, 3));
		GameObject* sphere = scene->GetObject(id);
		Primitive* primitive = sphere->AddComponent(new Primitive(PrimitiveType::Cylinder));
		primitive->colour = SDL_Color(0, 0, 1, 1);
		Physics::Collider* collider = sphere->AddComponent(new Physics::CylinderCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	//Add Camera controller
	GameObject* camObj = scene->GetCamera();
	///camObj->AddComponent(CameraController());
	camObj->SetPosition(Utilities::Vector3(0, 4, 15));
	camObj->SetRotation(Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward));

	//Test ressource manager
	Log::Normal(std::format("Ressource 0: {}", Engine::Instance->resources.GetFile(0).path));
	Log::Normal(std::format("Ressource \"test.txt\": {}", Resources::DataToText(Engine::Instance->resources.GetFile("test.txt").GetSDLData())));
	Log::Normal(std::format("Ressource \"test_2.txt\": {}", Engine::Instance->resources.GetFile("test_2.txt").GetStrData()));

	//Play audio
	ID audioId = scene->CreateObject("Audio Player");
	GameObject* audioPlayer = scene->GetObject(audioId);
	Audio::Emitter* emitter = audioPlayer->AddComponent(new Audio::Emitter("audio.wav", false));
	///Audio::Emitter emitter2 = Audio::Emitter("audio.wav", true);
	///emitter2.Play();

	//Export scene
	scene->ExportToFile();

	//Start engine
	engine.Start();
}