#include "main/Engine.hpp"
#include "inputs/InputSystem.hpp"
#include "main/Component.hpp"
#include "main/GameObject.hpp"
#include "main/Log.hpp"
#include "main/SceneManager.hpp"
#include "networking/networking.hpp"
#include "networking/server.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Vector3.hpp"

#include <string>
#include <SDL_keycode.h>

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
	GameObject& gameObject = GetParent();
	//Move position
	Utilities::Quaternion rotation = gameObject.GetWorldRotation();
	Utilities::Vector3 position = gameObject.GetPosition();
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
	gameObject.SetPosition(position);
	//Look around
	auto delta = inputManager.GetMouseDelta();
	Utilities::Quaternion localRotation = gameObject.GetRotation();
	localRotation = Quaternion::FromAngleAxis(-inputManager.GetMouseDelta().X * sensitivity * deltaTime, Vector3::up) * localRotation;
	localRotation = localRotation * Quaternion::FromAngleAxis(-inputManager.GetMouseDelta().Y * sensitivity * deltaTime, Vector3::right);
	gameObject.SetRotation(localRotation);
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

//Create engine
int main(int argc, char** argv) {
	CreateEngine("NetworkingServer", {
		#ifdef StevEngine_SHOW_WINDOW
		.vsync = true, .fullscreen = false,
		#endif
		.targetFPS = 100
	});
	Scene& scene = sceneManager.CreateScene("Debug scene");

	//Add Camera controller
	#ifdef StevEngine_SHOW_WINDOW
	GameObject& camObj = scene.GetCameraObject();
	camObj.AddComponent(new CameraController());
	camObj.SetPosition(Utilities::Vector3(0, 5, 20));
	camObj.SetRotation(Utilities::Quaternion::FromAngleAxis(Utilities::Quaternion::DegreesToRadians(0), Utilities::Vector3::forward));
	Camera* cam = scene.GetCamera();
	cam->isOrthographic = false;
	cam->zoom = 1;
	#endif

	//Setup networking
	Networking::Server::Manager server(5001);

	server.listen(10, [](const Networking::Server::Client& client, Networking::MessageData msg) {
		Log::Normal(std::string("From (") + client.id.GetString() +  "): \"" + msg.Read<std::string>() + "\"");
	});

	//Start engine
	return engine->Start();
}

int wmain(int argc, char** argv) {
	return main(argc, argv);
}
