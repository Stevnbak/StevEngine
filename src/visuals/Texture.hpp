#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/ResourceManager.hpp"

namespace StevEngine {
    namespace Visuals {
        class Texture {
            public:
                Texture(Resources::Resource file);
                ~Texture();
                int BindTexture();
                const static Texture empty;
            private:
                Texture() {};
                SDL_Surface* surface;
                unsigned int GLLocation;
        };
    }
}
#endif
