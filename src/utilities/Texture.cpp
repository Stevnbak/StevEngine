#ifdef StevEngine_RENDERER_GL
#include "Texture.hpp"
#ifdef WINDOWS
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
#include <SDL.h>

namespace StevEngine {
    namespace Utilities {
        Texture::Texture(SDL_RWops* data) {
            surface = IMG_Load_RW(data, 1);
        }
        Texture::~Texture() {}
        void Texture::Free() {
            if(surface) {
                SDL_FreeSurface(surface);
                surface = NULL;
            }
        }
    }
}
#endif
