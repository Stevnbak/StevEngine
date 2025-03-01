#include "main/Log.hpp"
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
	Texture::Texture(const Texture& copy) : path(copy.path), GLLocation(copy.GLLocation), surface(copy.surface), bound(copy.bound) {}
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
		if(IsBound()) FreeTexture();
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
		if(!IsBound()) return Log::Error("Texture is not bound!", true);
		glDeleteTextures(1, &GLLocation);
		bound = false;
	}

	ComputeTexture::ComputeTexture(uint32_t width, uint32_t height, GLenum format) : width(width), height(height), format(format), bound(false) {}
	ComputeTexture::ComputeTexture(const ComputeTexture& copy)
		: width(copy.width), height(copy.height), format(copy.format), GLLocation(copy.GLLocation), bound(copy.bound) {}
	ComputeTexture::~ComputeTexture() {}
	void ComputeTexture::BindTexture() {
		glCreateTextures(GL_TEXTURE_2D, 1, &GLLocation);
		glTextureStorage2D(GLLocation, 1, format, width, height);
		glTextureParameteri(GLLocation, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(GLLocation, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(GLLocation, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GLLocation, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//Set bound
		bound = true;
	}
	void ComputeTexture::FreeTexture() {
		if(!IsBound()) return Log::Error("Texture is not bound!", true);
		glDeleteTextures(1, &GLLocation);
		bound = false;
	}
	bool ComputeTexture::AttachToFrameBuffer(uint32_t framebuffer, GLenum attachmentType) {
		if(!IsBound()) {
			Log::Error("Texture is not bound!", true);
			return false;
		}
		glNamedFramebufferTexture(framebuffer, attachmentType, GLLocation, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Log::Error("Framebuffer is not complete!", true);
			return false;
		}
		return true;
	}
	void* ComputeTexture::RetrieveData(GLenum format, GLenum pixel, size_t dataSize) const {
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, buffer);
		glNamedBufferData(buffer,
			width * height * dataSize,
			(void*)0,
			GL_DYNAMIC_DRAW);
		// bind texture
		glBindTexture(GL_TEXTURE_2D, GLLocation);
		// transfer texture into PBO
		glGetTexImage(GL_TEXTURE_2D, 0, format, pixel, (GLvoid*)0);

		//map buffer to array
		return glMapNamedBuffer(buffer, GL_READ_ONLY);
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
	//Read from stream
	template <> Visuals::ComputeTexture Stream::Read<Visuals::ComputeTexture>() {
		return Visuals::ComputeTexture(Read<uint32_t>(),Read<uint32_t>());
	}
	//Write to stream
	template <> void Stream::Write<Visuals::ComputeTexture>(const Visuals::ComputeTexture& data) {
		*this << data.width << data.height;
	}
}
#endif
