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
extern const char _binary____assets_cube_xml_start[];
extern const char _binary____assets_cube_xml_end[];
extern const char _binary____assets_test_xml_start[];
extern const char _binary____assets_test_xml_end[];

int main(int argc, char** argv) {
	//Create engine
	Engine engine = Engine("StevnGame", 100, false, mainUpdate);
	//Setup debug stuff:
	Log::Normal("Start");
	glClearColor(0, 0, 0, 1);
	
	//Add debug assets
	engine.resources->AddFile("test.txt", &_binary____assets_test_txt_start[0], _binary____assets_test_txt_end - _binary____assets_test_txt_start);
	engine.resources->AddFile("test_2.txt", &_binary____assets_test_2_txt_start[0], _binary____assets_test_2_txt_end - _binary____assets_test_2_txt_start);
	engine.resources->AddFile("audio.wav", &_binary____assets_audio_wav_start[0], _binary____assets_audio_wav_end - _binary____assets_audio_wav_start);
	engine.resources->AddFile("cube.xml", &_binary____assets_cube_xml_start[0], _binary____assets_cube_xml_end - _binary____assets_cube_xml_start);
	engine.resources->AddFile("test.xml", &_binary____assets_test_xml_start[0], _binary____assets_test_xml_end - _binary____assets_test_xml_start);

	//Create test objects
	{
		GameObject* floor = GameObject::Create("Cube", Utilities::Vector3(0, -1, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(15), Utilities::Vector3::forward), Utilities::Vector3(100, 1, 100));
		Primitive* primitive = floor->AddComponent(new Primitive(PrimitiveType::Cube));
		primitive->colour = SDL_Color(0, 1, 0, 1);
		Physics::CubeCollider* collider = floor->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = floor->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
	}
	{
		GameObject* cube = GameObject::Create("Cube", Utilities::Vector3(0, 4, 0), Utilities::Quaternion(), Utilities::Vector3(1, 2, 1));
		Primitive* primitive = cube->AddComponent(new Primitive(PrimitiveType::Cube));
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::CubeCollider* collider = cube->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = cube->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		rb->SetMotionProperties(Physics::MotionProperties(1, 0.5));
		JPH::AABox bounds = collider->shape->GetLocalBounds();
		Utilities::Vector3 center = collider->shape->GetCenterOfMass();
		std::string test = cube->Export();
		cube->ExportToFile("cube.xml");
		Log::Normal(std::string("XML Cube Export: ") + test);
		GameObject* cube2 = GameObject::CreateFromFile(Engine::Instance->resources->GetFile("cube.xml"));
		Physics::Collider* col = cube2->GetComponent<Physics::Collider>();
		JPH::AABox bounds2 = col->shape->GetLocalBounds();
		Utilities::Vector3 center2 = col->shape->GetCenterOfMass();
		Primitive* primitive2 = cube2->GetComponent<Primitive>();
		primitive2->colour = SDL_Color(1, 1, 1, 1);
	}
	{
		GameObject* sphere = GameObject::Create("Sphere", Utilities::Vector3(3, 3, 0));
		Primitive* primitive = sphere->AddComponent(new Primitive(PrimitiveType::Sphere));
		primitive->colour = SDL_Color(1, 0, 0, 1);
		Physics::Collider* collider = sphere->AddComponent(new Physics::SphereCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
	}
	{
		GameObject* sphere = GameObject::Create("Cylinder", Utilities::Vector3(0, 3, 3));
		Primitive* primitive = sphere->AddComponent(new Primitive(PrimitiveType::Cylinder));
		primitive->colour = SDL_Color(0, 0, 1, 1);
		Physics::Collider* collider = sphere->AddComponent(new Physics::CylinderCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		///sphere->Destroy();
	}
	//Add Camera controller
	///Engine::Instance->activeCamera->gameObject->AddComponent(new CameraController());
	Engine::Instance->activeCamera->gameObject->SetPosition(Utilities::Vector3(0, 4, 15));
	Engine::Instance->activeCamera->gameObject->SetRotation(Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward));

	//Test ressource manager
	Log::Normal(std::format("Ressource 0: {}", Engine::Instance->resources->GetFile(0).path));
	Log::Normal(std::format("Ressource \"test.txt\": {}", Resources::DataToText(Engine::Instance->resources->GetFile("test.txt").GetSDLData())));
	Log::Normal(std::format("Ressource \"test_2.txt\": {}", Engine::Instance->resources->GetFile("test_2.txt").GetStrData()));

	//Play audio
	GameObject* audioPlayer = GameObject::Create("Audio Player");
	Audio::Emitter* emitter = audioPlayer->AddComponent(new Audio::Emitter("audio.wav", false));
	///Audio::Emitter emitter2 = Audio::Emitter("audio.wav", true);
	emitter->Play();
	///emitter2.Play();

	//Export gameobject
	GameObject* object = GameObject::Create("Test object", Utilities::Vector3(2, 3, 4));
	object->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving"), 2));
	std::string test = object->Export();
	Log::Normal(std::string("XML Export: ") + test);
	const char* t = test.c_str();

	GameObject* object2 = GameObject::CreateFromFile(Engine::Instance->resources->GetFile("test.xml"));

	//Start engine
	engine.Start();
}