#pragma once
#ifdef StevEngine_RENDERER_GL
#include "render/RenderComponent.hpp"

namespace StevEngine {
	namespace Visuals {
		enum TextureType {
			REPEAT,
			COVER
		};
		class CubePrimitive : public Render::RenderComponent {
			public:
				CubePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), TextureType textureType = COVER);
				CubePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
		};
		inline bool cube = CreateComponents::RegisterComponentType<CubePrimitive>("CubePrimitive");
		class UVSpherePrimitive : public Render::RenderComponent {
			public:
				UVSpherePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), bool smooth = true, TextureType textureType = COVER);
				UVSpherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool sphere = CreateComponents::RegisterComponentType<UVSpherePrimitive>("IcospherePrimitive");
		class IcospherePrimitive : public Render::RenderComponent {
			public:
				IcospherePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), bool smooth = true, TextureType textureType = COVER);
				IcospherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool icosphere = CreateComponents::RegisterComponentType<IcospherePrimitive>("IcospherePrimitive");
		class CylinderPrimitive : public Render::RenderComponent {
			public:
				CylinderPrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), bool smooth = true, TextureType textureType = COVER);
				CylinderPrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool cylinder = CreateComponents::RegisterComponentType<CylinderPrimitive>("CylinderPrimitive");
		class CapsulePrimitive : public Render::RenderComponent {
			public:
				CapsulePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), bool smooth = true, TextureType textureType = COVER);
				CapsulePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool capsule = CreateComponents::RegisterComponentType<CapsulePrimitive>("CapsulePrimitive");
	}
}
#endif
