#include "audio/Emitter.hpp"
#include "data/DataManager.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "inputs/InputSystem.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "main/Component.hpp"
#include "main/GameObject.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Colliders.hpp"
#include "physics/Layers.hpp"
#include "main/SceneManager.hpp"
#include "utilities/ID.hpp"
#include "utilities/Matrix4.hpp"
#include "utilities/Random.hpp"
#include "utilities/Stream.hpp"
#include "utilities/Terrain.hpp"
#include "utilities/Vector2.hpp"
#include "visuals/Material.hpp"
#include "visuals/Primitive.hpp"
#include "visuals/ModelRenderer.hpp"
#include "visuals/TerrainRenderer.hpp"
#include "visuals/renderer/RenderComponent.hpp"
#include "visuals/Lights.hpp"
#include "utilities/Quaternion.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Color.hpp"
#include "visuals/renderer/RenderSystem.hpp"
#include "visuals/shaders/Shader.hpp"

#include <cmath>
#include <fstream>
#include <string>
#include <SDL_keycode.h>
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
		CameraController() {};
		CameraController(Stream& stream) {};
		std::string GetType() const { return "Rotate"; }
		Utilities::Stream Export(Utilities::StreamType type) const { return Stream(type); }
};
bool camController = CreateComponents::RegisterComponentType<CameraController>("CameraController");
void CameraController::Update(double deltaTime) {
	if (inputManager.cursorMode == CursorMode::Free) {
		return;
	}
	GameObject* gameObject = GetParent();
	//Move position
	Utilities::Quaternion rotation = gameObject->GetWorldRotation();
	Utilities::Vector3 position = gameObject->GetPosition();
	Utilities::Vector3 forward = rotation.Forward();
	Utilities::Vector3 right = rotation.Right();
	Utilities::Vector3 up = rotation.Up();

	if (inputManager.IsKeyPressed(SDLK_w)) {
		position += forward * movementSpeed * deltaTime;
	}
	if (inputManager.IsKeyPressed(SDLK_s)) {
		position -= forward * movementSpeed * deltaTime;
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
	auto delta = inputManager.GetMouseDelta();
	Utilities::Quaternion localRotation = gameObject->GetRotation();
	localRotation = Quaternion::FromAngleAxis(-inputManager.GetMouseDelta().X * sensitivity * deltaTime, Vector3::up) * localRotation;
	localRotation = localRotation * Quaternion::FromAngleAxis(-inputManager.GetMouseDelta().Y * sensitivity * deltaTime, Vector3::right);
	gameObject->SetRotation(localRotation);
}
void CameraController::Start() {
	inputManager.cursorMode = CursorMode::Free;
	inputManager.GetEvents()->Subscribe<InputKeyDownEvent>([](InputKeyDownEvent event) {
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
		void Draw(const Utilities::Matrix4& transform) {}
		void Update(double deltaTime) {
			GameObject* gameObject = GetParent();
			Utilities::Quaternion rotation = gameObject->GetRotation();
			rotation *= Quaternion::FromAngleAxis(movementSpeed * deltaTime, axis);
			gameObject->SetRotation(rotation);
		}
		void Start() {};
		void Deactivate() {};
		std::string GetType() const { return "Rotate"; }
		Utilities::Stream Export(Utilities::StreamType type) const { return Stream(type); }
		Rotate() {};
		Rotate(Vector3 axis, double movementSpeed = 0.5) : axis(axis), movementSpeed(movementSpeed) {};
		Rotate(Stream& node) {};
};
bool rotate = CreateComponents::RegisterComponentType<Rotate>("Rotate");

void TestSerializeStuff() {
	Utilities::Vector3 a(2,3.523,8.92);
	Utilities::Vector3 b(5,0,9.9999);
	Utilities::Vector3 c(1,-20,0);
	ID id;

	Log::Debug("Text stream test:");
	Stream ts(Text);

	Log::Debug("Original: " + (std::string)a);
	ts.Write(a);
	Log::Debug("Txt stream: " + (std::string)ts.Read<Utilities::Vector3>());

	Log::Debug("Original: " + (std::string)b);
	ts << b;

	Log::Debug("Original: " + (std::string)c);
	ts.Write(c);

	Log::Debug("Txt stream: " + (std::string)ts.Read<Utilities::Vector3>());
	Log::Debug("Txt stream: " + (std::string)ts.Read<Utilities::Vector3>());

	std::string str("This is a test string!");
	Log::Debug("Original: " + str);
	ts << str;
	Log::Debug("Txt stream: " + ts.Read<std::string>());


	Log::Debug("Original: " + id.GetString());
	ts << id;
	Log::Debug("Txt stream: " + ts.Read<Utilities::ID>().GetString());

	ts.WriteToFile((Data::data.GetAppdataPath() + "TestExport.txt").c_str());

	Log::Debug("Binary stream test:");
	Stream bs(Binary);
	char chr = 'T';
	Log::Debug(std::string("Original: ") + chr);
	bs << chr;
	Log::Debug(std::string("Bin stream: ") + bs.Read<char>());

	Log::Debug("Original: " + (std::string)a);
	bs.Write(a);
	Log::Debug("Bin stream: " + (std::string)bs.Read<Utilities::Vector3>());

	Log::Debug("Original: " + (std::string)b);
	bs.Write(b);

	Log::Debug("Original: " + (std::string)c);
	bs.Write(c);

	Log::Debug("Bin stream: " + (std::string)bs.Read<Utilities::Vector3>());
	Log::Debug("Bin stream: " + (std::string)bs.Read<Utilities::Vector3>());

	Log::Debug("Original: " + str);
	bs << str;
	Log::Debug("Bin stream: " + bs.Read<std::string>());

	Log::Debug("Original: " + id.GetString());
	bs << id;
	Log::Debug("Bin stream: " + bs.Read<Utilities::ID>().GetString());
	bs.Write<Vector3>(Vector3(1.52,2,3));
	Log::Debug("Bin stream: " + (std::string)bs.Read<Vector3>());
	bs.WriteToFile((Data::data.GetAppdataPath() + "TestExport.bin").c_str());
}

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
	});

	TestSerializeStuff();

	Renderer::render.SetFaceCulling(false);
	Renderer::render.SetMSAA(true, 8);
	engine->GetEvents()->Subscribe<UpdateEvent>(mainUpdate);
	Renderer::render.SetFaceCulling(false);
	//Debug logging:
	Log::Debug("Debug log");
	Log::Warning("Warning log");
	Log::Error("Error log");

	//Add debug assets
	auto fs = cmrc::debug_development_assets::get_filesystem();
	for (std::string path : {"test.txt", "test_2.txt", "audio.wav", "cube.object", "Debug scene.scene", "Fox.stl", "cube.stl", "box.png", "prototype.png", "test_shader.frag", "normal_map.png"}) {
		cmrc::file file = fs.open("debug/development/assets/" + path);
		Resources::resourceManager.AddFile(path, file.begin(), file.size());
	}

	//Create test shader
	Renderer::ShaderProgram shader = Renderer::ShaderProgram(Renderer::FRAGMENT);
	shader.AddShader(Renderer::Shader(Resources::resourceManager.GetFile("test_shader.frag").GetRawData(), Renderer::FRAGMENT));

	//Create new scene
	//Scene* importedscene = sceneManager.CreateSceneFromFile(Resources::resourceManager.GetFile("Debug scene.scene"));
	///*
	Scene* scene = sceneManager.CreateScene("Debug scene");

	//sceneManager.SetActiveScene("Debug imported scene");

	//Create test objects
	{
		/*ID id = scene->CreateObject("Cube", Utilities::Vector3(0, -1, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward), Utilities::Vector3(100, 1, 100));
		GameObject* floor = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		CubePrimitive* primitive = floor->AddComponent(new CubePrimitive());
		primitive->GetMaterial().color = Color(0, 255, 0, 255 );
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::CubeCollider* collider = floor->AddComponent(new Physics::CubeCollider());
		Physics::RigidBody* rb = floor->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
		#endif*/
	}
	{
		ID id = scene->CreateObject("Sphere", Utilities::Vector3(0, 2, 0), Utilities::Quaternion(), Utilities::Vector3(1.0));
		GameObject* object = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		Renderer::RenderComponent* primitive = object->AddComponent(new IcospherePrimitive(Vector3(), Quaternion(), Vector3(1.0), Material(Color(255, 255, 255)), false));
		primitive->AddShader(shader);
		primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("box.png")));
		//primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		//object->AddComponent(new Rotate(Vector3::up));
		object->AddComponent(new Rotate(Vector3::right));
		#endif
		GameObject* parent = scene->GetObject(scene->CreateObject("Parent", Utilities::Vector3(0, 3, 0), Utilities::Quaternion(), Utilities::Vector3(1.0)));
		parent->AddChild(id);
		parent->AddComponent(new Rotate(Vector3::forward));
	}
	{
		GameObject* cube = scene->GetObject(scene->CreateObject("Cube", Utilities::Vector3(3, 2, 0), Utilities::Quaternion(), Utilities::Vector3(2.0)));
		#ifdef StevEngine_RENDERER_GL
		Renderer::RenderComponent* primitive = cube->AddComponent(new CubePrimitive(Vector3(), Quaternion(), Vector3(1.0), Material(Color(255, 255, 255))));
		primitive->AddShader(shader);
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		//cube->SetRotation(Quaternion::FromAngleAxis(Quaternion::DegreesToRadians(45), Vector3::up));
		//cube->AddComponent(new Rotate(Vector3::up));
		//cube->AddComponent(new Rotate(Vector3::right));
		#endif
	}
	{
		GameObject* cube = scene->GetObject(scene->CreateObject("Cylinder", Utilities::Vector3(-3, 2, 0), Utilities::Quaternion(), Utilities::Vector3(2.0)));
		#ifdef StevEngine_RENDERER_GL
		Renderer::RenderComponent* primitive = cube->AddComponent(new CylinderPrimitive(Vector3(), Quaternion(), Vector3(1), Material(Color(255, 255, 255)), false));
		primitive->AddShader(shader);
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		//primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		//cube->AddComponent(new Rotate(Vector3::up));
		//cube->AddComponent(new Rotate(Vector3::right));
		#endif
	}
	{
		GameObject* cube = scene->GetObject(scene->CreateObject("Capsule", Utilities::Vector3(-6, 2, 0), Utilities::Quaternion(), Utilities::Vector3(2.0)));
		#ifdef StevEngine_RENDERER_GL
		Renderer::RenderComponent* primitive = cube->AddComponent(new CapsulePrimitive(Vector3(), Quaternion(), Vector3(1), Material(Color(255, 255, 255)), false));
		primitive->AddShader(shader);
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		//primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		//cube->AddComponent(new Rotate(Vector3::up));
		//cube->AddComponent(new Rotate(Vector3::right));
		#endif
	}
	{
		GameObject* cube = scene->GetObject(scene->CreateObject("Icosphere", Utilities::Vector3(6, 2, 0), Utilities::Quaternion(), Utilities::Vector3(2.0)));
		#ifdef StevEngine_RENDERER_GL
		Renderer::RenderComponent* primitive = cube->AddComponent(new IcospherePrimitive(Vector3(), Quaternion(), Vector3(1), Material(Color(255, 255, 255)), false));
		primitive->AddShader(shader);
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		//primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		//cube->AddComponent(new Rotate(Vector3::up));
		//cube->AddComponent(new Rotate(Vector3::right));
		#endif
	}
	{
		GameObject* cube = scene->GetObject(scene->CreateObject("UVSphere", Utilities::Vector3(0, 2, 0), Utilities::Quaternion(), Utilities::Vector3(2.0)));
		#ifdef StevEngine_RENDERER_GL
		Renderer::RenderComponent* primitive = cube->AddComponent(new UVSpherePrimitive(Vector3(), Quaternion(), Vector3(1), Material(Color(255, 255, 255))));
		primitive->AddShader(shader);
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		//primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		//cube->AddComponent(new Rotate(Vector3::up));
		//cube->AddComponent(new Rotate(Vector3::right));
		#endif
	}
	//*
	{
		ID id = scene->CreateObject("Sphere", Utilities::Vector3(3, 3, 0), Utilities::Quaternion(), Utilities::Vector3(2.0));
		GameObject* sphere = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		UVSpherePrimitive* primitive = sphere->AddComponent(new UVSpherePrimitive(Vector3(), Quaternion(), Vector3(1), Material(Color(255, 0, 255))));
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		Material t = Material(Color(255, 0, 255));
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
		Model model = Model(Resources::resourceManager.GetFile("Fox.stl"));
		double modelScale = 1.0 / 30.0;
		#endif
		#ifdef StevEngine_RENDERER_GL
		ModelRenderer* r = modelObj->AddComponent(new ModelRenderer(model, Material(Color(255,255,0))));
		r->scale = Vector3(modelScale);
		#endif
		#ifdef StevEngine_PHYSICS
		modelObj->AddComponent(new Physics::ModelCollider(model, true, Vector3(), Quaternion(), Vector3(modelScale)));
		modelObj->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		ID id = scene->CreateObject("Cylinder", Utilities::Vector3(0, 3, 3), Quaternion(), Vector3(2.0));
		GameObject* obj = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		CylinderPrimitive* primitive = obj->AddComponent(new CylinderPrimitive(Vector3(), Quaternion(), Vector3(1), Material(Color(255, 255, 255, 255))));
		//primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("box.png")));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = obj->AddComponent(new Physics::CylinderCollider());
		Physics::RigidBody* rb = obj->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	{
		ID id = scene->CreateObject("Capsule", Utilities::Vector3(0, 3, 3), Quaternion(), Vector3(1.0, 1.0, 1.0));
		GameObject* obj = scene->GetObject(id);
		#ifdef StevEngine_RENDERER_GL
		CapsulePrimitive* primitive = obj->AddComponent(new CapsulePrimitive(Vector3(), Quaternion(), Vector3(1.0), Material(Color(255, 255, 255, 255)), true, TextureType::COVER));
		primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("box.png")));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::Collider* collider = obj->AddComponent(new Physics::CapsuleCollider());
		Physics::RigidBody* rb = obj->AddComponent(new Physics::RigidBody(JPH::EMotionType::Dynamic, Physics::Layer::GetLayerByName("Moving")));
		#endif
	}
	//Terrain
	if(true) {
		ID id = scene->CreateObject("Terrain", Utilities::Vector3(0, -1, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward), Utilities::Vector3(10, 1, 10));
		GameObject* object = scene->GetObject(id);
		double data[4*4];
		double height1 = 0;
		double height2 = 0;
		double height3 = 0;
		double height4 = 0;
		data[0] = height1;data[1] = height2;data[2] = height3;data[3] = height4;
		data[4] = height1;data[5] = height2;data[6] = height3;data[7] = height4;
		data[8] = height1;data[9] = height2;data[10] = height3;data[11] = height4;
		data[12] = height1;data[13] = height2;data[14] = height3;data[15] = height4;
		TerrainData terrain(4, 3, data);
		#ifdef StevEngine_RENDERER_GL
		TerrainRenderer* primitive = object->AddComponent(new TerrainRenderer(terrain, Material(Color(0, 255, 125, 255), Vector3(1), Vector3(0.75), Vector3(0.5), 1), true));
		//primitive->AddShader(shader);
		primitive->GetMaterial().SetAlbedo(Texture(Resources::resourceManager.GetFile("prototype.png")));
		primitive->GetMaterial().SetNormal(Texture(Resources::resourceManager.GetFile("normal_map.png")));
		#endif
		#ifdef StevEngine_PHYSICS
		Physics::TerrainCollider* collider = object->AddComponent(new Physics::TerrainCollider(terrain));
		Physics::RigidBody* rb = object->AddComponent(new Physics::RigidBody(JPH::EMotionType::Static, Physics::Layer::GetLayerByName("Static")));
		#endif
	}
	//*/
	//Add Camera controller
	#ifdef StevEngine_SHOW_WINDOW
	GameObject* camObj = scene->GetCameraObject();
	camObj->AddComponent(new CameraController());
	camObj->SetPosition(Utilities::Vector3(0, 5, 20));
	camObj->SetRotation(Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward));
	Camera* cam = scene->GetCamera();
	cam->isOrthographic = false;
	cam->zoom = 1;
	#endif

	//Add test lights
	#ifdef StevEngine_RENDERER_GL
	GameObject* light1 = scene->GetObject(scene->CreateObject("PointLight1", Utilities::Vector3(0, 5, 0)));
	light1->AddComponent(new Visuals::PointLight(Vector3(1.0), Vector3(0.5), 1, 0.05, 0.0017));
	GameObject* light2 = scene->GetObject(scene->CreateObject("PointLight2", Utilities::Vector3(5, 5, 3)));
	light2->AddComponent(new Visuals::PointLight(Vector3(1.0, 0.5, 0), Vector3(1.0, 0.5, 0) * 0.5));
	GameObject* light3 = scene->GetObject(scene->CreateObject("DirectionalLight", Utilities::Vector3(0, 0, 0), Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(45), Utilities::Vector3::right)));
	light3->AddComponent(new Visuals::DirectionalLight(Vector3(0.75), Vector3(0.5)));
	#endif
	//Test ressource manager
	Log::Debug(std::format("Ressource 0: {}", Resources::resourceManager.GetFile(0).path));
	Log::Debug(std::format("Ressource \"test.txt\": {}", Resources::resourceManager.GetFile("test.txt").GetStrData()));
	Log::Debug(std::format("Ressource \"test_2.txt\": {}", Resources::resourceManager.GetFile("test_2.txt").GetStrData()));

	//Use test shader
	#ifdef StevEngine_RENDERER_GL
	//Renderer::render.AddGlobalShader(shader);
	//scene->GetObject(modelObject)->GetComponent<Renderer::RenderComponent>()->AddShader(shader);
	#endif

	//Test data manager
	#ifdef StevEngine_PLAYER_DATA
	Log::Debug("Before: " + Data::data.Read<std::string>("test"));
	Data::data.Save<std::string>("test", std::string("test data"));
	Log::Debug("After: " + Data::data.Read<std::string>("test"));
	#endif

	//Play audio
	#ifdef StevEngine_AUDIO
	ID audioId = scene->CreateObject("Audio Player");
	GameObject* audioPlayer = scene->GetObject(audioId);
	Audio::Emitter* emitter = audioPlayer->AddComponent(new Audio::Emitter("audio.wav", false, 0.5));
	emitter->Play();
	#endif

	//Test graphics settings
	#ifdef StevEngine_SHOW_WINDOW
	/*inputManager.GetEvents()->Subscribe<InputKeyDownEvent>([](InputKeyDownEvent event) {
		if (event.key == SDLK_f) {
			StevEngine::engine->SetFullscreen(!engine->GetGameSettings().fullscreen);
		}
		else if (event.key == SDLK_v) {
			StevEngine::engine->SetVSync(!engine->GetGameSettings().vsync);
		}
	});*/
	#endif

	//Export scene
	#ifdef StevEngine_PLAYER_DATA
	scene->Export(Text).WriteToFile((Data::data.GetAppdataPath() + scene->name + ".scene").c_str());
	#endif
	//*/

	//Set background
	#ifdef StevEngine_RENDERER_GL
	Renderer::render.SetBackground(Color(0, 0, 0, 255));
	#endif

	//Start engine
	return engine->Start();
}

int wmain(int argc, char** argv) {
	return main(argc, argv);
}
