#pragma once
#ifdef StevEngine_RENDERER_GL
#include <SDL.h>

namespace StevEngine {
    namespace Utilities {
        class Texture {
            public:
                Texture(SDL_RWops* data);
                ~Texture();
                void Free();
                operator SDL_Surface*() { return surface; }
            private:
                SDL_Surface* surface;
        };
    }
}
#endif
