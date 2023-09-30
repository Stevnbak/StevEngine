#pragma once
#include <vector>

namespace StevEngine {
	class Camera {
		public:
			std::vector<double> position;
			std::vector<double> rotation;
			bool isOrthographic;
			double zoom, aspect, nearClip = 1, farClip = 100;
			Camera(std::vector<double> pos, std::vector<double> rot, bool orthographic, double zoomValue, double aspectRatio);
			void UpdateView();
	};
}