#pragma once
#include "scenes/Component.hpp"

namespace StevEngine {
	class GameObject;
	class Engine;
	namespace Render {
		class RenderSystem;
	}

	namespace Visuals {
		class Camera : public Component {
			friend class StevEngine::GameObject;
			friend class StevEngine::Engine;
			friend class Render::RenderSystem;

			public:
				bool isOrthographic;
				double zoom, fov, nearClip = 1, farClip = 100;
				Camera(bool orthographic = false, double fov = 70, double zoomValue = 1);
				Camera(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
			private:
				static const bool unique = true;
				glm::mat4x4 GetView();
				glm::mat4x4 GetProjection();
				//Component functions
				void Start() {};
				void Deactivate() {};
				void Update(double deltaTime) {};
				void Draw(glm::mat4x4 transform) {};
		};
		inline bool camera = CreateComponents::RegisterComponentType<Camera>("Camera");
	}
}
