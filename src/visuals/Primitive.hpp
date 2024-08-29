#pragma once
#ifdef StevEngine_RENDERER_GL
#include "render/Component.hpp"

namespace StevEngine {
	namespace Visuals {
		enum TextureType {
			REPEAT,
			COVER
		};
		class CubePrimitive : public Render::RenderComponent {
			public:
				CubePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), TextureType textureType = COVER);
				CubePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				Visuals::TextureType textureType;
		};
		inline bool cube = CreateComponents::RegisterComponentType<CubePrimitive>("CubePrimitive");
		class SpherePrimitive : public Render::RenderComponent {
			public:
				SpherePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), TextureType textureType = COVER);
				SpherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				Visuals::TextureType textureType;
		};
		inline bool sphere = CreateComponents::RegisterComponentType<SpherePrimitive>("SpherePrimitive");
		class CylinderPrimitive : public Render::RenderComponent {
			public:
				CylinderPrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), TextureType textureType = COVER);
				CylinderPrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				Visuals::TextureType textureType;
		};
		inline bool cylinder = CreateComponents::RegisterComponentType<CylinderPrimitive>("CylinderPrimitive");
		class CapsulePrimitive : public Render::RenderComponent {
			public:
				CapsulePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), TextureType textureType = COVER);
				CapsulePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				Visuals::TextureType textureType;
		};
		inline bool capsule = CreateComponents::RegisterComponentType<CapsulePrimitive>("CapsulePrimitive");
	}
}
#endif
