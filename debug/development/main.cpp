#include "audio/Emitter.hpp"
#include "main/DataManager.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/InputSystem.hpp"
#include "main/ResourceManager.hpp"
#include "scenes/Component.hpp"
#include "scenes/GameObject.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Colliders.hpp"
#include "physics/Layers.hpp"
#include "scenes/SceneManager.hpp"
#include "visuals/Primitive.hpp"
#include "visuals/Model.hpp"
#include "visuals/render/Component.hpp"
#include "visuals/render/Lights.hpp"
#include "utilities/Quaternion.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Color.hpp"
#include "visuals/render/System.hpp"
#include "visuals/shaders/Shader.hpp"

#include <yaml-cpp/yaml.h>
#include <SDL_keycode.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(debug_development_assets);

using namespace StevEngine;
using namespace StevEngine::Utilities;

#ifdef StevEngine_SHOW_WINDOW
using namespace StevEngine::Visuals;
class CameraController final : public Component {
	public:
		double movementSpeed = 10;
		double sensitivity = 2;
		void Update(double deltaTime);
		void Start();
		void Deactivate() {};
		CameraController() : Component("CameraController") {};
		CameraController(YAML::Node node) : Component(node) {};
};
bool camController = CreateComponents::RegisterComponentType<CameraController>("CameraController");
void CameraController::Update(double deltaTime) {
	if (inputManager.cursorMode == CursorMode::Free) {
		return;
	}
	GameObject* gameObject = GetParent();
	//Move position
	Utilities::Quaternion rotation = gameObject->GetRotation();
	Utilities::Vector3 position = gameObject->GetPosition();
	Utilities::Vector3 forward = rotation.Forward();
	Utilities::Vector3 right = rotation.Right();
	Utilities::Vector3 up = rotation.Up();
	if (inputManager.IsKeyPressed(SDLK_w)) {
		position -= forward * movementSpeed * deltaTime;
	}
	if (inputManager.IsKeyPressed(SDLK_s)) {
		position += forward * movementSpeed * deltaTime;
	}
	if (inputManager.IsKeyPressed(SDLK_d)) {
		position += right * movementSpeed * deltaTime;
	}
	if (inputManager.IsKeyPressed(SDLK_a)) {
		position -= right * movementSpeed * deltaTime;
	}
	if (inputManager.IsKeyPressed(SDLK_SPACE)) {
		position += up * movementSpeed * deltaTime;
	}
	if (inputManager.IsKeyPressed(SDLK_LSHIFT)) {
		position -= up * movementSpeed * deltaTime;
	}
	gameObject->SetPosition(position);
	//Look around
	rotation *= Quaternion::FromAngleAxis(-inputManager.GetMouseDelta().X * sensitivity * deltaTime, up);
	rotation *= Quaternion::FromAngleAxis(inputManager.GetMouseDelta().Y * sensitivity * deltaTime, right);
	gameObject->SetRotation(rotation);
}
void CameraController::Start() {
	inputManager.cursorMode = CursorMode::Free;
	inputManager.GetEvents().Subscribe<InputKeyDownEvent>([](InputKeyDownEvent event) {
		if (event.key == SDLK_ESCAPE) {
			if (inputManager.cursorMode == CursorMode::Free) {
				inputManager.cursorMode = CursorMode::Locked;
			}
			else {
				inputManager.cursorMode = CursorMode::Free;
				inputManager.cursorVisible = true;
			}
		}
	});
}
#endif

class Rotate final : public Component {
	public:
		double movementSpeed = 0.5;
		Vector3 axis = Vector3::right;
		void Draw(const glm::mat4x4& transform) {}
		void Update(double deltaTime) {
			GameObject* gameObject = GetParent();
			Utilities::Quaternion rotation = gameObject->GetRotation();
			rotation *= Quaternion::FromAngleAxis(movementSpeed * deltaTime, axis);
			gameObject->SetRotation(rotation);
		}
		void Start() {};
		void Deactivate() {};
		Rotate() : Component("Rotate") {};
		Rotate(Vector3 axis, double movementSpeed = 0.5) : axis(axis), movementSpeed(movementSpeed), Component("Rotate") {};
		Rotate(YAML::Node node) : Component(node) {};
};
bool rotate = CreateComponents::RegisterComponentType<Rotate>("Rotate");

ID modelObject;

void mainUpdate(UpdateEvent event) {
	/*Utilities::Quaternion testQ(0, 0, 90);
	Utilities::Vector3 forward = testQ.forward();
	Log::Normal(std::format("Forward: ({};{};{})", forward.X, forward.Y, forward.Z));
	Utilities::Vector3 right = testQ.right();
	Log::Normal(std::format("Right: ({};{};{})", right.X, right.Y, right.Z));
	Utilities::Vector3 up = testQ.up();
	Log::Normal(std::format("Up: ({};{};{})", up.X, up.Y, up.Z));*/
	/*GameObject* model = sceneManager.GetActiveScene()->GetObject(modelObject);
	model->SetRotation(model->GetRotation() * Quaternion::FromAngleAxis(1 * deltaTime, Vector3(0,1,0)));//*/
}

//Create engine
int main(int argc, char** argv) {
	CreateEngine("Debug", {
		#ifdef StevEngine_SHOW_WINDOW
		.vsync = true, .fullscreen = false,
		#endif
		.targetFPS = 100
	}, false);
	engine->GetEvents().Subscribe<UpdateEvent>(mainUpdate);
	Render::render.SetFaceCulling(false);
	//Debug logging:
	Log::Debug("Debug log");
	Log::Warning("Warning log");
	Log::Error("Error log");

	//Add debug assets
	auto fs = cmrc::debug_development_assets::get_filesystem();
	for (std::string path : {"test.txt", "test_2.txt", "audio.wav", "cube.object", "Debug scene.scene", "Fox.stl", "cube.stl", "box.png", "prototype.png", "test_shader.frag"}) {
		cmrc::file file = fs.open("debug/development/assets/" + path);
		Resources::resourceManager.AddFile(path, file.begin(), file.size());
	}

	//Create new scene
	//Scene& importedscene = sceneManager.CreateSceneFromFile(Resources::resourceManager.GetFile("Debug scene.scene"));
	///*
	Scene& scene = sceneManager.CreateScene("Debug scene");

	//sceneManager.SetActiveScene("Debug imported scene");

	//Create test objects
	{
		GameObject& object = scene.CreateObject("Cube", Utilities::Vector3(0, -1, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward), Utilities::Vector3(100, 1, 100));
		#ifdef StevEngine_RENDERER_GL
		CubePrimitive* primitive = object.AddComponent(new CubePrimitive());
		primitive->SetColor(Color(0, 255, 0, 255 ));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::CubeCollider* collider = object.AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = object.AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
		#endif
	}
	{
		GameObject& object = scene.CreateObject("Cube", Utilities::Vector3(0, 4, 0), Utilities::Quaternion(), Utilities::Vector3(2.0));
		#ifdef StevEngine_RENDERER_GL
		Render::RenderComponent* primitive = object.AddComponent(new CubePrimitive(Vector3(), Quaternion(), Vector3(1.0), TextureType::REPEAT));
		primitive->SetColor(Color(255, 255, 255, 255));
		primitive->SetTexture(Texture(Resources::resourceManager.GetFile("prototype.png")));
		object.AddComponent(new Rotate(Vector3::up));
		object.AddComponent(new Rotate(Vector3::right));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::CubeCollider* collider = object.AddComponent(new Physics::CubeCollider());
		//Physics::RigidBody* rb = cube->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
		GameObject& parent = scene.CreateObject("CubeParent", Utilities::Vector3(5, 0, 0), Utilities::Quaternion(), Utilities::Vector3(1.0));
		parent.AddChild(object.Id());
	}
	{
		GameObject& object = scene.CreateObject("Sphere", Utilities::Vector3(3, 3, 0), Utilities::Quaternion(), Utilities::Vector3(2.0));
		#ifdef StevEngine_RENDERER_GL
		SpherePrimitive* primitive = object.AddComponent(new SpherePrimitive());
		primitive->SetColor(Color(255, 255, 255, 255));
		primitive->SetTexture(Texture(Resources::resourceManager.GetFile("prototype.png")));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = object.AddComponent(new Physics::SphereCollider());
		Physics::RigidBody* rb = object.AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		GameObject& modelObj = scene.CreateObject("Model", Utilities::Vector3(0, 0, 0));
		modelObject = modelObj.Id();
		#ifdef StevEngine_MODELS
		Model model = Model(Resources::resourceManager.GetFile("Fox.stl"));
		double modelScale = 1.0 / 30.0;
		#endif
		#ifdef StevEngine_RENDERER_GL
		ModelRenderer* r = modelObj.AddComponent(new ModelRenderer(model));
		r->scale = Vector3(modelScale);
		r->SetColor({255,255,0,255});
		#endif
		#ifdef StevEngine_PHYSICS
		modelObj.AddComponent(new Physics::ModelCollider(model, true, Vector3(), Quaternion(), Vector3(modelScale)));
		modelObj.AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		GameObject& object = scene.CreateObject("Cylinder", Utilities::Vector3(0, 3, 3), Quaternion(), Vector3(1.0, 1.0, 1.0));
		#ifdef StevEngine_RENDERER_GL
		CylinderPrimitive* primitive = object.AddComponent(new CylinderPrimitive());
		primitive->SetColor(Color(255, 255, 255, 255));
		primitive->SetTexture(Texture(Resources::resourceManager.GetFile("box.png")));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = object.AddComponent(new Physics::CylinderCollider());
		Physics::RigidBody* rb = object.AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		GameObject& object = scene.CreateObject("Capsule", Utilities::Vector3(0, 3, 3), Quaternion(), Vector3(1.0, 1.0, 1.0));
		#ifdef StevEngine_RENDERER_GL
		CapsulePrimitive* primitive = object.AddComponent(new CapsulePrimitive(Vector3(), Quaternion(), Vector3(1.0), TextureType::COVER));
		primitive->SetColor(Color(255, 255, 255, 255));
		primitive->SetTexture(Texture(Resources::resourceManager.GetFile("box.png")));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = object.AddComponent(new Physics::CapsuleCollider());
		Physics::RigidBody* rb = object.AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	//Add Camera controller
	#ifdef StevEngine_SHOW_WINDOW
	GameObject* camObj = scene.GetCameraObject();
	camObj->AddComponent(new CameraController());
	camObj->SetPosition(Utilities::Vector3(0, 4, 25));
	camObj->SetRotation(Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward));
	#endif

	//Add test lights
	#ifdef StevEngine_RENDERER_GL
	GameObject& light1 = scene.CreateObject("PointLight1", Utilities::Vector3(0, 8, 3));
	light1.AddComponent(new Render::PointLight(Vector3(0.0, 1.0, 1.0), Vector3(0.0, 1.0, 1.0) * 0.5));
	light1.AddComponent(new CubePrimitive(Utilities::Vector3(), Utilities::Quaternion(), Utilities::Vector3(0.1)))->SetColor({255,255,255,255});
	GameObject& light2 = scene.CreateObject("PointLight2", Utilities::Vector3(2, 5, 1));
	light2.AddComponent(new Render::PointLight(Vector3(1.0, 0.5, 0.0), Vector3(1.0, 0.5, 0.0) * 0.5));
	light2.AddComponent(new CubePrimitive(Utilities::Vector3(), Utilities::Quaternion(), Utilities::Vector3(0.1)));
	scene.CreateObject("DirectionalLight", Utilities::Vector3(0, 0, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(-90), Utilities::Vector3::right)).AddComponent(new Render::DirectionalLight(Vector3(0.33), Vector3(0.25)));
	#endif
	//Test ressource manager
	Log::Debug(std::format("Ressource 0: {}", Resources::resourceManager.GetFile(0).path));
	Log::Debug(std::format("Ressource \"test.txt\": {}", Resources::resourceManager.GetFile("test.txt").GetStrData()));
	Log::Debug(std::format("Ressource \"test_2.txt\": {}", Resources::resourceManager.GetFile("test_2.txt").GetStrData()));

	//Use test shader
	#ifdef StevEngine_RENDERER_GL
	Render::ShaderProgram shader = Render::ShaderProgram(Render::FRAGMENT);
	shader.AddShader(Render::Shader(Resources::resourceManager.GetFile("test_shader.frag").GetRawData(), Render::FRAGMENT));
	//Render::render.AddGlobalShader(shader);
	scene.GetObject(modelObject)->GetComponent<Render::RenderComponent>()->AddShader(shader);
	#endif

	//Test data manager
	#ifdef StevEngine_PLAYER_DATA
	Log::Debug("Before: " + data.Read<std::string>("test"));
	data.Save("test", std::string("test data"));
	Log::Debug("After: " + data.Read<std::string>("test"));
	#endif

	//Play audio
	#ifdef StevEngine_AUDIO
	GameObject& audioPlayer = scene.CreateObject("Audio Player");
	Audio::Emitter* emitter = audioPlayer.AddComponent(new Audio::Emitter("audio.wav", false, 0.5));
	emitter->Play();

	//engine->audio.PlayBackground("audio.wav", true);
	#endif

	//Test graphics settings
	#ifdef StevEngine_SHOW_WINDOW
	inputManager.GetEvents().Subscribe<InputKeyDownEvent>([](InputKeyDownEvent event) {
		if (event.key == SDLK_f) {
			StevEngine::engine->SetFullscreen(!engine->GetGameSettings().fullscreen);
		}
		else if (event.key == SDLK_v) {
			StevEngine::engine->SetVSync(!engine->GetGameSettings().vsync);
		}
	});
	#endif

	//Export scene
	#ifdef StevEngine_PLAYER_DATA
	scene.ExportToFile();
	#endif
	//*/

	//Set background
	#ifdef StevEngine_RENDERER_GL
	Render::render.SetBackground(Color(0, 0, 0, 255));
	#endif

	//Start engine
	return engine->Start();
}

int wmain(int argc, char** argv) {
	return main(argc, argv);
}
