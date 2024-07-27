#pragma once
#include <scenes/Component.hpp>

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
				double zoom, aspect, nearClip = 1, farClip = 100;
				Camera(bool orthographic, double zoomValue, double aspectRatio);
				Camera(tinyxml2::XMLElement* element);
			private:
				static const bool unique = true;
				glm::mat4x4 GetView();
				glm::mat4x4 GetProjection();
				//Component functions
				void Start() {};
				void Deactivate() {};
				void Update(double deltaTime) {};
				void Draw(glm::mat4x4 transform) {};
				void Export(tinyxml2::XMLElement* element);
		};
	}
}