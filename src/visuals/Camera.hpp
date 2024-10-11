#pragma once
#ifdef StevEngine_SHOW_WINDOW
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

			public:
				bool isOrthographic;
				double zoom, fov, nearClip = 1, farClip = 100;
				Camera(bool orthographic = false, double fov = 70, double zoomValue = 1);
				Camera(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				glm::mat4x4 GetView() const;
				glm::mat4x4 GetProjection() const;
			private:
				static const bool unique = true;
		};
		inline bool camera = CreateComponents::RegisterComponentType<Camera>("Camera");
	}
}
#endif
