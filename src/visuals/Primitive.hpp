#pragma once
#ifdef StevEngine_RENDERER_GL
#include "renderer/RenderComponent.hpp"
#include "visuals/Material.hpp"

namespace StevEngine {
	namespace Visuals {
		/**
		 * @brief Types of texture mapping for primitives
		 */
		enum TextureType {
			REPEAT,  ///< Texture repeats across surface
			COVER	///< Texture stretches to cover surface
		};

		/**
		 * @brief Cube primitive renderer
		 *
		 * Renders a textured cube with six faces.
		 * Supports different texture mapping modes.
		 */
		class CubePrimitive : public Renderer::RenderComponent {
			public:
				/**
				 * @brief Create cube primitive
				 * @param position Local position offset
				 * @param rotation Local rotation offset
				 * @param scale Local scale modifier
				 * @param material Surface material
				 * @param textureType Texture mapping mode
				 */
				CubePrimitive(Utilities::Vector3 position = Utilities::Vector3(),
							  Utilities::Quaternion rotation = Utilities::Quaternion(),
							  Utilities::Vector3 scale = Utilities::Vector3(1),
							  const Material& material = Material(),
							  TextureType textureType = REPEAT);

				/**
				 * @brief Create from serialized data
				 * @param node YAML node with primitive data
				 */
				CubePrimitive(YAML::Node node);

				/**
				 * @brief Serialize to YAML
				 * @param node Node to serialize into
				 * @return Updated YAML node
				 */
				YAML::Node Export(YAML::Node node) const;

				const Visuals::TextureType textureType;  ///< Texture mapping mode
		};

		/**
		 * @brief UV-mapped sphere primitive renderer
		 *
		 * Renders a sphere using UV mapping for textures.
		 * Can use smooth or flat shading.
		 */
		class UVSpherePrimitive : public Renderer::RenderComponent {
			public:
				/**
				 * @brief Create UV sphere primitive
				 * @param position Local position offset
				 * @param rotation Local rotation offset
				 * @param scale Local scale modifier
				 * @param material Surface material
				 * @param smooth Use smooth shading
				 * @param textureType Texture mapping mode
				 */
				UVSpherePrimitive(Utilities::Vector3 position = Utilities::Vector3(),
								  Utilities::Quaternion rotation = Utilities::Quaternion(),
								  Utilities::Vector3 scale = Utilities::Vector3(1),
								  Material material = Material(),
								  bool smooth = true,
								  TextureType textureType = COVER);

				UVSpherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;  ///< Texture mapping mode
				const bool smooth;					   ///< Whether to use smooth shading
		};

		/**
		 * @brief Icosphere primitive renderer
		 *
		 * Renders a sphere using subdivided icosahedron.
		 * Provides more uniform triangulation than UV sphere.
		 */
		class IcospherePrimitive : public Renderer::RenderComponent {
			public:
				/**
				 * @brief Create icosphere primitive
				 * @param position Local position offset
				 * @param rotation Local rotation offset
				 * @param scale Local scale modifier
				 * @param material Surface material
				 * @param smooth Use smooth shading
				 * @param textureType Texture mapping mode
				 */
				IcospherePrimitive(Utilities::Vector3 position = Utilities::Vector3(),
								   Utilities::Quaternion rotation = Utilities::Quaternion(),
								   Utilities::Vector3 scale = Utilities::Vector3(1),
								   Material material = Material(),
								   bool smooth = true,
								   TextureType textureType = COVER);

				IcospherePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;  ///< Texture mapping mode
				const bool smooth;					   ///< Whether to use smooth shading
		};

		/**
		 * @brief Cylinder primitive renderer
		 *
		 * Renders a cylinder with circular caps.
		 * Supports smooth or flat shading.
		 */
		class CylinderPrimitive : public Renderer::RenderComponent {
			public:
				/**
				 * @brief Create cylinder primitive
				 * @param position Local position offset
				 * @param rotation Local rotation offset
				 * @param scale Local scale modifier
				 * @param material Surface material
				 * @param smooth Use smooth shading
				 * @param textureType Texture mapping mode
				 */
				CylinderPrimitive(Utilities::Vector3 position = Utilities::Vector3(),
								  Utilities::Quaternion rotation = Utilities::Quaternion(),
								  Utilities::Vector3 scale = Utilities::Vector3(1),
								  Material material = Material(),
								  bool smooth = true,
								  TextureType textureType = COVER);

				CylinderPrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;  ///< Texture mapping mode
				const bool smooth;					   ///< Whether to use smooth shading
		};

		/**
		 * @brief Capsule primitive renderer
		 *
		 * Renders a capsule (cylinder with spherical ends).
		 * Supports smooth or flat shading.
		 */
		class CapsulePrimitive : public Renderer::RenderComponent {
			public:
				/**
				 * @brief Create capsule primitive
				 * @param position Local position offset
				 * @param rotation Local rotation offset
				 * @param scale Local scale modifier
				 * @param material Surface material
				 * @param smooth Use smooth shading
				 * @param textureType Texture mapping mode
				 */
				CapsulePrimitive(Utilities::Vector3 position = Utilities::Vector3(),
								 Utilities::Quaternion rotation = Utilities::Quaternion(),
								 Utilities::Vector3 scale = Utilities::Vector3(1),
								 Material material = Material(),
								 bool smooth = true,
								 TextureType textureType = COVER);

				CapsulePrimitive(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				const Visuals::TextureType textureType;  ///< Texture mapping mode
				const bool smooth;					   ///< Whether to use smooth shading
		};

		/** @brief Register primitive components */
		inline bool cube = CreateComponents::RegisterComponentType<CubePrimitive>("CubePrimitive");
		inline bool sphere = CreateComponents::RegisterComponentType<UVSpherePrimitive>("IcospherePrimitive");
		inline bool icosphere = CreateComponents::RegisterComponentType<IcospherePrimitive>("IcospherePrimitive");
		inline bool cylinder = CreateComponents::RegisterComponentType<CylinderPrimitive>("CylinderPrimitive");
		inline bool capsule = CreateComponents::RegisterComponentType<CapsulePrimitive>("CapsulePrimitive");
	}
}
#endif