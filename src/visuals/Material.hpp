#pragma once
#ifdef StevEngine_SHOW_WINDOW
#include "utilities/Vector3.hpp"
#include "utilities/Color.hpp"
#include "visuals/Texture.hpp"
#include <yaml-cpp/yaml.h>

namespace StevEngine::Visuals {
	class Material {
		public:
			//Constructor
			Material(
				Utilities::Color color = Utilities::Color(),
				Utilities::Vector3 ambient = Utilities::Vector3(1.0),
				Utilities::Vector3 diffuse = Utilities::Vector3(1.0),
				Utilities::Vector3 specular = Utilities::Vector3(1.0),
				float shininess = 32.0,
				const Visuals::Texture& albedoData = Visuals::Texture::empty,
				const Visuals::Texture& normalData = Visuals::Texture::empty
			);
			Material(const Material& other);
			//Color
			Utilities::Color color;
			//Light effects
			Utilities::Vector3 ambient;
			Utilities::Vector3 diffuse;
			Utilities::Vector3 specular;
			float shininess;
			//Textures
			const Visuals::Texture& GetAlbedo() const { return albedo; };
			void SetAlbedo(const Texture& textureData);
			void FreeAlbedo();
			const Visuals::Texture& GetNormal() const { return normal; };
			void SetNormal(const Texture& normalData);
			void FreeNormal();
		private:
			Texture albedo = Texture::empty;
			Texture normal = Texture::empty;
		//Import/Export
		public:
			Material(YAML::Node node);
			YAML::Node Export() const;
	};
}
#endif