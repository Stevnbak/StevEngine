#pragma once
#include <scenes/Component.hpp>

namespace StevEngine {
	class GameObject;
	class Engine;
	class Camera : public Component {
		friend class StevEngine::GameObject;
		friend class StevEngine::Engine;
		public:
			bool isOrthographic;
			double zoom, aspect, nearClip = 1, farClip = 100;
			Camera(bool orthographic, double zoomValue, double aspectRatio);
			Camera(tinyxml2::XMLElement* element);
		private:
			static const bool unique = true;
			//Component functions
			void UpdateView();
			void Start() {};
			void Deactivate() {};
			void Update(double deltaTime) {};
			void Draw() {};
			void Export(tinyxml2::XMLElement* element);
	};
}