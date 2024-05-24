#pragma once
#include <Core/Utilities.hpp>
#include <Core/GameObject.hpp>
using namespace StevEngine::Utilities;

namespace StevEngine {
	class Camera : public Component {
		public:
			bool isOrthographic;
			double zoom, aspect, nearClip = 1, farClip = 100;
			void SetOptions(bool orthographic, double zoomValue, double aspectRatio);
			void UpdateView();
			//Component functions
			void Start() {};
			void Update(double deltaTime) {};
			void Draw() {};
	};
}