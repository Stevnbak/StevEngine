#pragma once
#ifdef StevEngine_SHOW_WINDOW
#include "utilities/Vector3.hpp"
#include "utilities/Color.hpp"
#include "visuals/Texture.hpp"
#include <yaml-cpp/yaml.h>

namespace StevEngine::Visuals {
	/**
	 * @brief Material properties for rendering
	 *
	 * Defines surface properties including color, textures, and light interaction.
	 * Controls how objects appear when rendered.
	 */
	class Material {
		public:
			/**
			 * @brief Create material with properties
			 * @param color Base color tint
			 * @param ambient Ambient light reflection
			 * @param diffuse Diffuse light reflection
			 * @param specular Specular light reflection
			 * @param shininess Specular highlight size
			 * @param albedoData Albedo/color texture
			 * @param normalData Normal map texture
			 */
			Material(
				Utilities::Color color = Utilities::Color(),
				Utilities::Vector3 ambient = Utilities::Vector3(1.0),
				Utilities::Vector3 diffuse = Utilities::Vector3(1.0),
				Utilities::Vector3 specular = Utilities::Vector3(1.0),
				float shininess = 32.0,
				const Visuals::Texture& albedoData = Visuals::Texture::empty,
				const Visuals::Texture& normalData = Visuals::Texture::empty
			);

			/**
			 * @brief Copy constructor
			 * @param other Material to copy
			 */
			Material(const Material& other);

			/** @brief Base color tint */
			Utilities::Color color;

			/** @brief Ambient light reflection coefficient */
			Utilities::Vector3 ambient;
			/** @brief Diffuse light reflection coefficient */
			Utilities::Vector3 diffuse;
			/** @brief Specular light reflection coefficient */
			Utilities::Vector3 specular;
			/** @brief Specular highlight size/sharpness */
			float shininess;

			/**
			 * @brief Get albedo/color texture
			 * @return Reference to albedo texture
			 */
			const Visuals::Texture& GetAlbedo() const { return albedo; };

			/**
			 * @brief Set albedo/color texture
			 * @param textureData New albedo texture
			 */
			void SetAlbedo(const Texture& textureData);

			/**
			 * @brief Free albedo texture resources
			 */
			void FreeAlbedo();

			/**
			 * @brief Get normal map texture
			 * @return Reference to normal map
			 */
			const Visuals::Texture& GetNormal() const { return normal; };

			/**
			 * @brief Set normal map texture
			 * @param normalData New normal map
			 */
			void SetNormal(const Texture& normalData);

			/**
			 * @brief Free normal map resources
			 */
			void FreeNormal();

		private:
			Texture albedo = Texture::empty;  ///< Albedo/color texture
			Texture normal = Texture::empty;  ///< Normal map texture

		//Import/Export
		public:
			/**
			 * @brief Create material from serialized data
			 * @param node YAML node with material data
			 */
			Material(YAML::Node node);

			/**
			 * @brief Serialize material to YAML
			 * @return YAML node containing material data
			 */
			YAML::Node Export() const;
	};
}
#endif