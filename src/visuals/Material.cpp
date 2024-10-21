#ifdef StevEngine_SHOW_WINDOW
#include "Material.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Vector3.hpp"

using namespace StevEngine::Utilities;

namespace StevEngine::Visuals {
	//Constructor
	Material::Material(Color color, Vector3 ambient, Vector3 diffuse, Vector3 specular, float shininess, const Texture& albedoData, const Texture& normalData)
		: color(color), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), albedo(albedoData), normal(normalData)
	{
		albedo.BindTexture();
		normal.BindTexture();
	}

	Material::Material(const Material& other)
	: color(other.color), ambient(other.ambient), diffuse(other.diffuse), specular(other.specular), shininess(other.shininess), albedo(other.albedo), normal(other.normal) {}

	//Textures
	void Material::SetAlbedo(const Texture& albedoData) {
		albedo = albedoData;
		albedo.BindTexture();
	}
	void Material::SetNormal(const Texture& normalData) {
		normal = normalData;
		normal.BindTexture();
	}
	void Material::FreeAlbedo() {
		albedo.FreeTexture();
	}
	void Material::FreeNormal() {
		normal.FreeTexture();
	}

	//Import/Export
	Material::Material(YAML::Node node) :
		color(node["color"].as<Color>()),
		ambient(node["ambient"].as<Vector3>()),
		diffuse(node["diffuse"].as<Vector3>()),
		specular(node["specular"].as<Vector3>()),
		shininess(node["shininess"].as<float>()),
		albedo(Texture::empty), normal(Texture::empty)
	{
		if(node["albedo"].as<std::string>() != "") SetAlbedo(Visuals::Texture(Resources::resourceManager.GetFile(node["albedo"].as<std::string>())));
		if(node["normal"].as<std::string>() != "") SetNormal(Visuals::Texture(Resources::resourceManager.GetFile(node["normal"].as<std::string>())));
	}
	YAML::Node Material::Export() const {
		YAML::Node node;
		node["color"] = color;
		node["ambient"] = ambient;
		node["diffuse"] = diffuse;
		node["specular"] = specular;
		node["shininess"] = shininess;
		node["albedo"] = albedo.GetPath();
		node["normal"] = normal.GetPath();
		return node;
	}
}
#endif