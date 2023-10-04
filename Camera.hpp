#pragma once
#include "Utilities.hpp"
using namespace StevEngine::Utilities;

namespace StevEngine {
	class Camera {
		public:
			Vector3d position;
			Vector3d rotation;
			bool isOrthographic;
			double zoom, aspect, nearClip = 1, farClip = 100;
			Camera(Vector3d pos, Vector3d rot, bool orthographic, double zoomValue, double aspectRatio);
			void UpdateView();
	};
}