#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/ResourceManager.hpp"
#include "glad/gl.h"

namespace StevEngine::Visuals {
	/**
	 * @brief OpenGL texture wrapper
	 *
	 * Manages loading, binding and cleanup of OpenGL textures.
	 * Supports loading from image files and provides texture state management.
	 */
	class Texture {
		public:
			/**
			 * @brief Load texture from resource
			 * @param file Resource containing image data
			 */
			Texture(Resources::Resource file);

			/**
			 * @brief Copy constructor
			 * @param copy Texture to copy
			 */
			Texture(const Texture& copy);

			/**
			 * @brief Assignment operator
			 * @param copy Texture to copy from
			 */
			void operator=(const Texture& copy);

			/**
			 * @brief Clean up texture resources
			 */
			~Texture();

			/**
			 * @brief Bind texture to OpenGL
			 * Generates and configures texture in GPU memory
			 */
			void BindTexture();

			/**
			 * @brief Free texture from OpenGL
			 * Releases texture from GPU memory
			 */
			void FreeTexture();

			/**
			 * @brief Check if texture is bound to OpenGL
			 * @return True if texture is in GPU memory
			 */
			bool IsBound() const { return bound; };

			/**
			 * @brief Get OpenGL texture ID
			 * @return OpenGL texture location
			 */
			GLuint GetGLLocation() const { return GLLocation; };

			/**
			 * @brief Get texture file path
			 * @return Path to source texture file
			 */
			std::string GetPath() const { return path; }

			/** @brief Empty texture singleton */
			const static Texture empty;

		private:
			/** @brief Create empty texture */
			Texture() : bound(false), surface(nullptr), GLLocation(0) {};

			std::string path;		 ///< Path to texture file
			SDL_Surface* surface;	 ///< SDL surface containing image data
			GLuint GLLocation;	   ///< OpenGL texture ID
			bool bound;			  ///< Whether texture is bound to OpenGL
	};
}
#endif
