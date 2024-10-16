#pragma once
#include "glad/gl.h"
#ifdef StevEngine_RENDERER_GL
#include "main/ResourceManager.hpp"

namespace StevEngine {
	namespace Visuals {
		class Texture {
			public:
				Texture(Resources::Resource file);
				Texture(const Texture& copy);
				void operator=(const Texture& copy);
				~Texture();
				void BindTexture();
				void FreeTexture();
				bool IsBound() const { return bound; };
				GLuint GetGLLocation() const { return GLLocation; };
				std::string GetPath() const { return path; }
				const static Texture empty;
			private:
				Texture() : bound(false), surface(nullptr), GLLocation(0) {};
				std::string path;
				SDL_Surface* surface;
				GLuint GLLocation;
				bool bound;
		};
	}
}
#endif
