#pragma once
#include <core/Utilities.hpp>
#include <core/GameObject.hpp>
using namespace StevEngine::Utilities;

namespace StevEngine {
	class Camera : public Component {
		public:
			bool isOrthographic;
			double zoom, aspect, nearClip = 1, farClip = 100;
			Camera(bool orthographic, double zoomValue, double aspectRatio);
			Camera(tinyxml2::XMLElement* element);
			void UpdateView();
			//Component functions
			void Start() {};
			void Update(double deltaTime) {};
			void Draw() {};
			void Export(tinyxml2::XMLElement* element);
	};
}