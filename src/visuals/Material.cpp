#ifdef StevEngine_SHOW_WINDOW
#include "Material.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Stream.hpp"

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
}

namespace StevEngine {
	//Read from stream
	template <> Visuals::Material Stream::Read<Visuals::Material>() {
		return Visuals::Material(
			Read<Color>(),
			Read<Vector3>(),
			Read<Vector3>(),
			Read<Vector3>(),
			Read<float>(),
			Read<Visuals::Texture>(),
			Read<Visuals::Texture>()
		);
	}
	//Write to stream
	template <> void Stream::Write<Visuals::Material>(const Visuals::Material& data) {
		*this << data.color << data.ambient << data.diffuse << data.specular << data.shininess << data.GetAlbedo() << data.GetNormal();
	}
}
#endif