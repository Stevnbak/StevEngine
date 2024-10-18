#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "utilities/Color.hpp"
#include "visuals/shaders/Shader.hpp"
#include "visuals/shaders/ShaderProgram.hpp"
#include "main/EventSystem.hpp"

#include <SDL.h>
#include <glm/mat4x4.hpp>

#include <vector>
#include <array>
#include <type_traits>
#include <cstddef>
#include <cstdint>

namespace StevEngine {
	namespace Render {
		//Lights
		class Light;
		class DirectionalLight;
		class PointLight;
		class SpotLight;

		//Object structs
		struct RenderObject {
			const CustomObject& object;
			const glm::mat4x4 transform;
			RenderObject(const CustomObject& object, const glm::mat4x4& transform) : object(object), transform(transform) {}
			void Draw() {
				object.Draw(transform);
			}
		};

		//Render queues
		enum RenderQueue {
			STANDARD,
			TRANSPARENT,
			OVERLAY,

			MUST_BE_LAST
		};

		//System
		class RenderSystem {
			friend class StevEngine::Render::Light;
			friend class StevEngine::Render::DirectionalLight;
			friend class StevEngine::Render::PointLight;
			friend class StevEngine::Render::SpotLight;

			public:
				void Init(SDL_Window* window);
				void DrawObject(const CustomObject& object, glm::mat4x4 transform, RenderQueue queue = STANDARD);
				void SetBackground(const Utilities::Color& color);
				void SetAmbientLight(float strength, const Utilities::Color& color = Utilities::Color(255,255,255,255));
				void AddGlobalShader(ShaderProgram shader);
				void ResetGlobalShader(ShaderType type);
				//For Engine
				static const Uint32 WindowType();
				void DrawFrame();
				void SetViewSize(int WIDTH, int HEIGHT);
				void SetVSync(bool vsync);
				void SetFaceCulling(bool enable, GLenum face = GL_BACK, bool clockwise = false);
				void SetMSAA(bool enable, uint16_t amount = 4);
				//Get info
				const ShaderProgram& GetDefaultVertexShaderProgram() const { return vertexShaderProgram; }
				const ShaderProgram& GetDefaultFragmentShaderProgram() const { return fragmentShaderProgram; }
				uint32_t GetShaderPipeline() const { return shaderPipeline; }
				Utilities::Color GetAmbientLightColor() const { return ambientLightColor; }
				float GetAmbientLightStrength() const { return ambientLightStrength; }
				//From Lights
				uint32_t GetLightID(std::string type);
				std::vector<Light*> GetLights() const { return lights; }
			private:
				SDL_GLContext context;
				//Queues
				std::array<std::vector<RenderObject>, RenderQueue::MUST_BE_LAST> queues;
				//Shader program
				ShaderProgram vertexShaderProgram;
				ShaderProgram fragmentShaderProgram;
				uint32_t shaderPipeline;
				//GPU Buffers
				uint32_t VBO; //Vertex Buffer Object
				uint32_t EBO; //Element Buffer Object
				uint32_t VAO; //Vertex Array Object
				//Background
				Utilities::Color backgroundColor = {0, 0, 0, 255};
				//Lights
				std::vector<Light*> lights;
				Utilities::Color ambientLightColor;
				float ambientLightStrength;
		};

		extern RenderSystem render;
	}
}
#endif
