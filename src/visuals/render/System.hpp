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

namespace StevEngine {
	namespace Render {
		//Lights
		class Light;
		class DirectionalLight;
		class PointLight;
		class SpotLight;

		//Object structs
		struct RenderObject {
			const Object& object;
			glm::mat4x4 transform;
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
				void DrawObject(const Object& object, glm::mat4x4 transform, RenderQueue queue = STANDARD);
				void SetBackground(Utilities::Color color);
				void SetAmbientLight(float strength, Utilities::Color color = Utilities::Color(255,255,255,255));
				void AddGlobalShader(ShaderProgram shader);
				void ResetGlobalShader(ShaderType type);
			public:
				//For Engine
				static const Uint32 WindowType();
				void DrawFrame();
				void SetViewSize(int WIDTH, int HEIGHT);
				void SetVSync(bool vsync);
				void SetFaceCulling(bool enable, GLenum face = GL_BACK, bool clockwise = false);
				//From Lights
				std::vector<Light*> lights;
				unsigned int GetLightID(std::string type);
			private:
				SDL_GLContext context;
				//Queues
				std::array<std::vector<RenderObject>, RenderQueue::MUST_BE_LAST> queues;
				void Draw(RenderObject object);
				//Shader program
				ShaderProgram vertexShaderProgram;
				ShaderProgram fragmentShaderProgram;
				unsigned int shaderPipeline;
				//GPU Buffers
				unsigned int VBO; //Vertex Buffer Object
				unsigned int EBO; //Element Buffer Object
				unsigned int VAO; //Vertex Array Object
				//Background
				Utilities::Color backgroundColor = {0, 0, 0, 255};
		};

		extern RenderSystem render;
	}
}
#endif
