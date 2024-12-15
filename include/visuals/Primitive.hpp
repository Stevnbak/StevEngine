#pragma once
#ifdef StevEngine_RENDERER_GL
#include "renderer/RenderComponent.hpp"
#include "visuals/Material.hpp"

namespace StevEngine {
	namespace Visuals {
		enum TextureType {
			REPEAT,
			COVER
		};
		class CubePrimitive : public Renderer::RenderComponent {
			public:
				CubePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), const Material& material = Material(), TextureType textureType = REPEAT);
				CubePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
		};
		inline bool cube = CreateComponents::RegisterComponentType<CubePrimitive>("CubePrimitive");
		class UVSpherePrimitive : public Renderer::RenderComponent {
			public:
				UVSpherePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), Material material = Material(), bool smooth = true, TextureType textureType = COVER);
				UVSpherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool sphere = CreateComponents::RegisterComponentType<UVSpherePrimitive>("IcospherePrimitive");
		class IcospherePrimitive : public Renderer::RenderComponent {
			public:
				IcospherePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), Material material = Material(), bool smooth = true, TextureType textureType = COVER);
				IcospherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool icosphere = CreateComponents::RegisterComponentType<IcospherePrimitive>("IcospherePrimitive");
		class CylinderPrimitive : public Renderer::RenderComponent {
			public:
				CylinderPrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), Material material = Material(), bool smooth = true, TextureType textureType = COVER);
				CylinderPrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool cylinder = CreateComponents::RegisterComponentType<CylinderPrimitive>("CylinderPrimitive");
		class CapsulePrimitive : public Renderer::RenderComponent {
			public:
				CapsulePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1), Material material = Material(), bool smooth = true, TextureType textureType = COVER);
				CapsulePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;
				const bool smooth;
		};
		inline bool capsule = CreateComponents::RegisterComponentType<CapsulePrimitive>("CapsulePrimitive");
	}
}
#endif
