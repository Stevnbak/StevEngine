#include <main/Engine.hpp>
#include <scenes/GameObject.hpp>
#include <visuals/Primitive.hpp>
#include <physics/RigidBody.hpp>
#include <physics/Colliders.hpp>
#include <physics/Layers.hpp>
#include <main/ResourceManager.hpp>

#include <assets.h>
#include "visuals/Model.hpp"

using namespace StevEngine;
using namespace StevEngine::Utilities;
using namespace StevEngine::Visuals;

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

ID modelObject;

void mainUpdate(double deltaTime) {
	/*Utilities::Quaternion testQ(0, 0, 90);
	Utilities::Vector3 forward = testQ.forward();
	Log::Normal(std::format("Forward: ({};{};{})", forward.X, forward.Y, forward.Z));
	Utilities::Vector3 right = testQ.right();
	Log::Normal(std::format("Right: ({};{};{})", right.X, right.Y, right.Z));
	Utilities::Vector3 up = testQ.up();
	Log::Normal(std::format("Up: ({};{};{})", up.X, up.Y, up.Z));*/
	/*GameObject* model = Engine::Instance->scenes.GetActiveScene()->GetObject(modelObject);
	model->SetRotation(model->GetRotation() * Quaternion::FromAngleAxis(1 * deltaTime, Vector3(0,1,0)));//*/
}

int main(int argc, char** argv) {
	//Create engine
	Engine engine = Engine("Debug", 60, mainUpdate);
	//Debug logging:
	Log::Debug("Debug log");
	Log::Warning("Warning log");
	Log::Error("Error log");

	//Add debug assets
	engine.resources.AddFileFromHex("test.txt", test_txt_data, test_txt_size);
	engine.resources.AddFileFromHex("test_2.txt", test_2_txt_data, test_2_txt_size);
	engine.resources.AddFileFromHex("audio.wav", audio_wav_data, audio_wav_size);
	engine.resources.AddFileFromHex("cube.object", cube_object_data, cube_object_size);
	engine.resources.AddFileFromHex("Debug_scene.scene", Debug_scene_scene_data, Debug_scene_scene_size);
	engine.resources.AddFileFromHex("Fox.stl", Fox_stl_data, Fox_stl_size);
	engine.resources.AddFileFromHex("cube.stl", cube_stl_data, cube_stl_size);

	//Create new scene
	//Scene* scene = engine.scenes.CreateSceneFromFile(engine.resources.GetFile("Debug_scene.scene"));
	Scene* scene = engine.scenes.CreateScene("Debug scene");

	//Create test objects
	{
		ID id = scene->CreateObject("Cube", Utilities::Vector3(0, -1, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward), Utilities::Vector3(100, 1, 100));
		GameObject* floor = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		CubePrimitive* primitive = floor->AddComponent(new CubePrimitive());
		primitive->SetColor((SDL_Color){0, 1, 0, 1});
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::CubeCollider* collider = floor->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = floor->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
		#endif
	}
	{
		ID id = scene->CreateObject("Cube", Utilities::Vector3(0, 4, 0), Utilities::Quaternion(), Utilities::Vector3(1, 2, 1));
		GameObject* cube = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		CubePrimitive* primitive = cube->AddComponent(new CubePrimitive());
		primitive->SetColor((SDL_Color){1, 0, 0, 1});
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::CubeCollider* collider = cube->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = cube->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
		cube->ExportToFile("cube");
		/*ID id2 = scene->CreateObjectFromFile(Engine::Instance->resources.GetFile("cube.object"));
		GameObject* cube2 = scene->GetObject(id2);
		#ifdef StevEngine_PHYSICS
		Physics::Collider* col = cube2->GetComponent<Physics::Collider>();
		#endif
		#ifdef StevEngine_RENDERER_GL
		CubePrimitive* primitive2 = cube2->GetComponent<CubePrimitive>();
		primitive2->SetColor(SDL_Color(1, 1, 1, 1));
		#endif*/
	}
	{
		ID id = scene->CreateObject("Sphere", Utilities::Vector3(3, 3, 0));
		GameObject* sphere = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		SpherePrimitive* primitive = sphere->AddComponent(new SpherePrimitive());
		primitive->SetColor((SDL_Color){1, 0, 0, 1});
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = sphere->AddComponent(new Physics::SphereCollider());
		Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		modelObject = scene->CreateObject("Model", Utilities::Vector3(0, 0, 0));
		GameObject* modelObj = scene->GetObject(modelObject);
		#ifdef StevEngine_MODELS
		Model model = Model(engine.resources.GetFile("Fox.stl"));
		double modelScale = 1.0 / 30.0;
		#endif
		#ifdef StevEngine_RENDERER_GL
		ModelRenderer* r = modelObj->AddComponent(new ModelRenderer(model));
		r->scale = Vector3(1.0, 1.0, 1.0) * modelScale;
		r->SetColor({1,1,0});
		#endif
		#ifdef StevEngine_PHYSICS
		modelObj->AddComponent(new Physics::ModelCollider(model, true, Vector3(), Quaternion(), Vector3(1.0, 1.0, 1.0) * modelScale));
		modelObj->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		ID id = scene->CreateObject("Cylinder", Utilities::Vector3(0, 3, 3));
		GameObject* sphere = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		CylinderPrimitive* primitive = sphere->AddComponent(new CylinderPrimitive());
		primitive->SetColor((SDL_Color){0, 0, 1, 1});
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = sphere->AddComponent(new Physics::CylinderCollider());
		//Physics::RigidBody* rb = sphere->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	//Add Camera controller
	GameObject* camObj = scene->GetCameraObject();
	///camObj->AddComponent(CameraController());
	camObj->SetPosition(Utilities::Vector3(0, 4, 25));
	camObj->SetRotation(Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward));

	//Test ressource manager
	Log::Debug(std::format("Ressource 0: {}", Engine::Instance->resources.GetFile(0).path));
	Log::Debug(std::format("Ressource \"test.txt\": {}", Engine::Instance->resources.GetFile("test.txt").GetStrData()));
	Log::Debug(std::format("Ressource \"test_2.txt\": {}", Engine::Instance->resources.GetFile("test_2.txt").GetStrData()));

	//Test data manager
	#ifdef StevEngine_PLAYER_DATA
	Log::Debug("Before: " + engine.data.ReadData("test"));
	engine.data.SaveData("test", "test data");
	Log::Debug("After: " + engine.data.ReadData("test"));
	#endif

	//Play audio
	#ifdef StevEngine_AUDIO
	ID audioId = scene->CreateObject("Audio Player");
	GameObject* audioPlayer = scene->GetObject(audioId);
	Audio::Emitter* emitter = audioPlayer->AddComponent(new Audio::Emitter("audio.wav", false, 0.5));
	emitter->Play();

	//engine.audio.PlayBackground("audio.wav", true);
	#endif

	//Export scene
	#ifdef StevEngine_PLAYER_DATA
	//scene->ExportToFile();
	#endif

	//Set background
	#ifdef StevEngine_RENDERER_GL
	engine.render.SetBackground((SDL_Color){0, 0, 0, 1});
	#endif

	//Start engine
	engine.Start();
}
