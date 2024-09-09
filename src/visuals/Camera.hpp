#pragma once
#include "scenes/Component.hpp"

namespace StevEngine {
	class GameObject;
	class Engine;
	namespace Render {
		class System;
	}

	namespace Visuals {
		class Camera : public Component {
			friend class StevEngine::GameObject;
			friend class StevEngine::Engine;
			friend class Render::System;

			public:
				bool isOrthographic;
				double zoom, nearClip = 1, farClip = 100;
				Camera(bool orthographic, double zoomValue);
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
