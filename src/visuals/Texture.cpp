#ifdef StevEngine_RENDERER_GL
#include "Texture.hpp"
#include "main/ResourceManager.hpp"
#include "main/Engine.hpp"
#ifdef WINDOWS
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
#include <SDL.h>

namespace StevEngine {
    namespace Visuals {
        const Texture Texture::empty = Texture();

        Texture::Texture(Resources::Resource file) {
            surface = IMG_Load_RW(file.GetSDLData(), true);
        }
        Texture::~Texture() {}
        int Texture::BindTexture() {
            if(!surface) return 0;
            glGenTextures(1, &GLLocation);
            glBindTexture(GL_TEXTURE_2D, GLLocation);
            //Genereate texture
            SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, convertedSurface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedSurface->pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
            //Free surface data
            SDL_FreeSurface(surface);
            SDL_FreeSurface(convertedSurface);
            return GLLocation;
        }
    }
}
#endif
