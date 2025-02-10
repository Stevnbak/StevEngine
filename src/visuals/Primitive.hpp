#pragma once
#ifdef StevEngine_RENDERER_GL
#include "renderer/RenderComponent.hpp"
#include "visuals/Material.hpp"

#define CUBE_PRIMITIVE_TYPE "CubePrimitive"
#define UVSPHERE_PRIMITIVE_TYPE "UVSpherePrimitive"
#define ICOSPHERE_PRIMITIVE_TYPE "IcoSpherePrimitive"
#define CYLINDER_PRIMITIVE_TYPE "CylinderPrimitive"
#define CAPSULE_PRIMITIVE_TYPE "CapsulePrimitive"

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
				 * @brief Create cube from serialized data
				 * @param stream Stream containing serialized component data
				 */
				CubePrimitive(Stream& stream);

				/**
				 * @brief Get component type
				 * @return Type identifier string
				 */
				std::string GetType() const { return CUBE_PRIMITIVE_TYPE; }

				/**
				 * @brief Serialize component to a stream
				 * @param type Type of stream to export to
				 * @return Serialized stream
				 */
				Stream Export(StreamType type) const;

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

				/**
				 * @brief Create cube from serialized data
				 * @param stream Stream containing serialized component data
				 */
				UVSpherePrimitive(Stream& stream);

				/**
				 * @brief Get component type
				 * @return Type identifier string
				 */
				std::string GetType() const { return UVSPHERE_PRIMITIVE_TYPE; }

				/**
				 * @brief Serialize component to a stream
				 * @param type Type of stream to export to
				 * @return Serialized stream
				 */
				Stream Export(StreamType type) const;

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

				/**
				 * @brief Create cube from serialized data
				 * @param stream Stream containing serialized component data
				 */
				IcospherePrimitive(Stream& stream);

				/**
				 * @brief Get component type
				 * @return Type identifier string
				 */
				std::string GetType() const { return ICOSPHERE_PRIMITIVE_TYPE; }

				/**
				 * @brief Serialize component to a stream
				 * @param type Type of stream to export to
				 * @return Serialized stream
				 */
				Stream Export(StreamType type) const;

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

				/**
				 * @brief Create cube from serialized data
				 * @param stream Stream containing serialized component data
				 */
				CylinderPrimitive(Stream& stream);

				/**
				 * @brief Get component type
				 * @return Type identifier string
				 */
				std::string GetType() const { return CYLINDER_PRIMITIVE_TYPE; }

				/**
				 * @brief Serialize component to a stream
				 * @param type Type of stream to export to
				 * @return Serialized stream
				 */
				Stream Export(StreamType type) const;

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

				/**
				 * @brief Create capsule from serialized data
				 * @param stream Stream containing serialized component data
				 */
				CapsulePrimitive(Stream& stream);

				/**
				 * @brief Get component type
				 * @return Type identifier string
				 */
				std::string GetType() const { return CAPSULE_PRIMITIVE_TYPE; }

				/**
				 * @brief Serialize component to a stream
				 * @param type Type of stream to export to
				 * @return Serialized stream
				 */
				Stream Export(StreamType type) const;

				const Visuals::TextureType textureType;  ///< Texture mapping mode
				const bool smooth;					   ///< Whether to use smooth shading
		};

		/** @brief Register primitive components */
		inline bool cube = CreateComponents::RegisterComponentType<CubePrimitive>(CUBE_PRIMITIVE_TYPE);
		inline bool sphere = CreateComponents::RegisterComponentType<UVSpherePrimitive>(UVSPHERE_PRIMITIVE_TYPE);
		inline bool icosphere = CreateComponents::RegisterComponentType<IcospherePrimitive>(ICOSPHERE_PRIMITIVE_TYPE);
		inline bool cylinder = CreateComponents::RegisterComponentType<CylinderPrimitive>(CYLINDER_PRIMITIVE_TYPE);
		inline bool capsule = CreateComponents::RegisterComponentType<CapsulePrimitive>(CAPSULE_PRIMITIVE_TYPE);
	}
}
#endif