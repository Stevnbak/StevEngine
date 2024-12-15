#pragma once
#include "utilities/Matrix4.hpp"
#ifdef StevEngine_SHOW_WINDOW
#include "main/Component.hpp"

namespace StevEngine {
	namespace Visuals {
		class Camera : public Component {
			public:
				Camera(bool orthographic = false, float fov = 70, float zoomValue = 1, float nearClip = 0.01, float farClip = 1000);
				Camera(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				Utilities::Matrix4 GetView() const;
				Utilities::Matrix4 GetProjection() const;
				bool isOrthographic;
				float zoom, fov, nearClip, farClip;
				static const bool unique = true;
		};
		inline bool camera = CreateComponents::RegisterComponentType<Camera>("Camera");
	}
}
#endif
