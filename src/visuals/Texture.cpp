#include "utilities/Stream.hpp"
#ifdef StevEngine_RENDERER_GL
#include "Texture.hpp"
#include "main/ResourceManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

namespace StevEngine::Visuals {
	const Texture Texture::empty = Texture();

	Texture::Texture(Resources::Resource file) : path(file.path), bound(false) {
		surface = IMG_Load_RW(file.GetSDLData(), true);
	}
	Texture::Texture(const Texture& copy) : path(copy.path), GLLocation(copy.GLLocation), surface(copy.surface), bound(false) {}
	void Texture::operator=(const Texture& copy) {
		if(bound) FreeTexture();
		GLLocation = copy.GLLocation;
		surface = copy.surface;
	}
	Texture::~Texture() {}
	void Texture::BindTexture() {
		if(!surface) {
			GLLocation = 0;
			return;
		};
		glGenTextures(1, &GLLocation);
		glBindTexture(GL_TEXTURE_2D, GLLocation);
		//Genereate texture
		SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, convertedSurface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		//Set bound
		bound = true;
		//Free surface data
		SDL_FreeSurface(surface);
		SDL_FreeSurface(convertedSurface);
	}
	void Texture::FreeTexture() {
		glDeleteTextures(1, &GLLocation);
		bound = false;
	}
}

namespace StevEngine::Utilities {
	//Read from stream
	template <> Visuals::Texture Stream::Read<Visuals::Texture>() {
		return Visuals::Texture(Resources::resourceManager.GetFile(Read<std::string>()));
	}
	//Write to stream
	template <> void Stream::Write<Visuals::Texture>(const Visuals::Texture& data) {
		*this << data.GetPath();
	}
}
#endif
