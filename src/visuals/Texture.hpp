#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/ResourceManager.hpp"

#include <glad/gl.h>

#include <cstdint>

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
			void BindTexture(bool force = false);

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

		protected:
			/** @brief Create empty texture */
			Texture() : bound(false), surface(nullptr), GLLocation(0) {};

			std::string path;		 ///< Path to texture file
			SDL_Surface* surface;	 ///< SDL surface containing image data
			GLuint GLLocation;	   ///< OpenGL texture ID
			bool bound;			  ///< Whether texture is bound to OpenGL
	};

	class ComputeTexture : public Texture {
		public:
			ComputeTexture(uint32_t width, uint32_t height, GLenum format = GL_RGBA32F);

			/**
			 * @brief Copy constructor
			 * @param copy Texture to copy
			 */
			ComputeTexture(const ComputeTexture& copy);

			~ComputeTexture();

			/**
			 * @brief Bind texture to OpenGL
			 * Generates and configures texture in GPU memory
			 */
			void BindTexture(bool force = false);

			/**
			 * @brief Attach to an OpenGL framebuffer
			 * Generates and configures texture in GPU memory
			 * @param framebuffer OpenGL location of the framebuffer to attach to
			 * @return true if attachment was succesful, false if not
			 */
			bool AttachToFrameBuffer(uint32_t framebuffer, GLenum attachmentType = GL_COLOR_ATTACHMENT0);

			/**
			 * @brief Retrieve pointer to the data from the texture
			 * @param format OpenGL pixel format for the returned data (@see https://docs.gl/gl4/glGetTexImage `format` parameter)
			 * @param dataSize Size of the data for each pixel. For GL_RGB and GL_FLOAT it should be `sizeof(float) * 3`
			 * @param pixel OpenGL pixel type for the returned data. (@see https://docs.gl/gl4/glGetTexImage `type` parameter)
			 * @return true if attachment was succesful, false if not
			 */
			void* RetrieveData(GLenum format, size_t dataSize = sizeof(float), GLenum pixel = GL_FLOAT) const;

			const uint32_t width;	///< Width of the texture
			const uint32_t height;	///< Height of the texture
			const GLenum format;	///< OpenGL image storage format
	};
}
#endif
