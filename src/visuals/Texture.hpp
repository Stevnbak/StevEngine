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
				const std::string path;
			private:
				Texture() {};
				SDL_Surface* surface;
				uint32_t GLLocation;
		};
	}
}
#endif
