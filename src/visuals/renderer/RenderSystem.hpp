#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "utilities/Color.hpp"
#include "visuals/shaders/Shader.hpp"
#include "visuals/shaders/ShaderProgram.hpp"

#include <SDL.h>

#include <vector>
#include <array>
#include <cstdint>

namespace StevEngine {
	namespace Visuals {
		class Light;
	}
	/**
	 * @brief Core rendering system
	 *
	 * Manages OpenGL rendering pipeline, window setup, and graphics state.
	 * Handles render queues, shaders, and frame drawing.
	 */
	namespace Renderer {
		//Object
		/**
		 * @brief Container for renderable object and transform
		 * Groups an object with its world transform for rendering
		 */
		struct RenderObject {
			const CustomObject& object;		  ///< Object to render
			const Utilities::Matrix4 transform;  ///< World transform matrix

			/**
			 * @brief Create render object
			 * @param object Object to render
			 * @param transform World transform matrix
			 */
			RenderObject(const CustomObject& object, const Utilities::Matrix4& transform) : object(object), transform(transform) {}

			/**
			 * @brief Draw the object
			 */
			void Draw() {
				object.Draw(transform);
			}
		};

		/**
		 * @brief Render queue types for sorting objects
		 */
		enum RenderQueue {
			STANDARD,	 ///< Default opaque objects
			TRANSPARENT,  ///< Transparent objects
			OVERLAY,	  ///< UI and overlay objects
			MUST_BE_LAST  ///< Queue count marker
		};

		/**
		 * @brief Core rendering system
		 *
		 * Manages OpenGL rendering pipeline, window setup, and graphics state.
		 * Handles render queues, shaders, and frame drawing.
		 */
		class RenderSystem {
			public:
				/**
				 * @brief Initialize rendering system
				 * @param window SDL window to render to
				 */
				void Init(SDL_Window* window);

				/**
				 * @brief Queue object for rendering
				 * @param object Object to render
				 * @param transform World transform matrix
				 * @param queue Queue to add to
				 */
				void DrawObject(const CustomObject& object, Utilities::Matrix4 transform, RenderQueue queue = STANDARD);

				/**
				 * @brief Set background clear color
				 * @param color New background color
				 */
				void SetBackground(const Utilities::Color& color);

				/**
				 * @brief Set ambient light properties
				 * @param strength Ambient light intensity
				 * @param color Ambient light color
				 */
				void SetAmbientLight(float strength, const Utilities::Color& color = Utilities::Color(255,255,255,255));

				/**
				 * @brief Add global shader program
				 * @param shader Shader program to add
				 */
				void AddGlobalShader(ShaderProgram shader);

				/**
				 * @brief Reset global shader to default
				 * @param type Type of shader to reset
				 */
				void ResetGlobalShader(ShaderType type);

				// Engine interface functions
				static const Uint32 WindowType();	///< Get SDL window flags
				void DrawFrame();					///< Draw a complete frame
				void SetViewSize(int WIDTH, int HEIGHT);  ///< Set viewport size
				void SetVSync(bool vsync);		   ///< Set vertical sync
				void SetFaceCulling(bool enable, GLenum face = GL_FRONT, bool clockwise = false);  ///< Set face culling
				void SetMSAA(bool enable, uint16_t amount = 4);  ///< Set anti-aliasing

				// Getters
				const ShaderProgram& GetDefaultVertexShaderProgram() const { return vertexShaderProgram; }
				const ShaderProgram& GetDefaultFragmentShaderProgram() const { return fragmentShaderProgram; }
				uint32_t GetShaderPipeline() const { return shaderPipeline; }
				Utilities::Color GetAmbientLightColor() const { return ambientLightColor; }
				float GetAmbientLightStrength() const { return ambientLightStrength; }

				// Light management
				/**
				 * @brief Get next available light ID for type
				 * @param type Light type string
				 * @return Available light index
				 */
				uint32_t GetLightID(std::string type);

				/**
				 * @brief Get all active lights
				 * @return Vector of light pointers
				 */
				std::vector<Visuals::Light*> GetLights() const { return lights; }

				/**
				 * @brief Add light to scene
				 * @param light Light to add
				 */
				void AddLight(Visuals::Light* light);

				/**
				 * @brief Remove light from scene
				 * @param light Light to remove
				 */
				void RemoveLight(Visuals::Light* light);

			private:
				SDL_GLContext context;  ///< OpenGL context

				/** @brief Render queues for different types of objects */
				std::array<std::vector<RenderObject>, RenderQueue::MUST_BE_LAST> queues;

				// Shader programs
				ShaderProgram vertexShaderProgram;	///< Default vertex shader
				ShaderProgram fragmentShaderProgram;  ///< Default fragment shader
				uint32_t shaderPipeline;			 ///< Current shader pipeline

				// GPU Buffers
				uint32_t VBO;  ///< Vertex Buffer Object
				uint32_t EBO;  ///< Element Buffer Object
				uint32_t VAO;  ///< Vertex Array Object

				// Scene properties
				Utilities::Color backgroundColor = {0, 0, 0, 255};  ///< Background clear color
				std::vector<Visuals::Light*> lights;  ///< Active lights
				Utilities::Color ambientLightColor;   ///< Ambient light color
				float ambientLightStrength;		  ///< Ambient light intensity
		};

		extern RenderSystem render;  ///< Global render system instance
	}
}
#endif
