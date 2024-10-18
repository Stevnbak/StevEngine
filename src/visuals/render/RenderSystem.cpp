#ifdef StevEngine_RENDERER_GL
#include "RenderSystem.hpp"
#include "main/Log.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Settings.hpp"
#include "scenes/SceneManager.hpp"
#include "visuals/render/Object.hpp"
#include "visuals/render/Lights.hpp"
#include "visuals/shaders/Shader.hpp"
#include "visuals/shaders/ShaderProgram.hpp"
#include "visuals/Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>

namespace StevEngine {
	namespace Render {
		const char* vertexShaderSource =
			#include "visuals/shaders/default.vert"
		;
		const char* fragmentShaderSource =
			#include "visuals/shaders/default.frag"
		;

		RenderSystem render = RenderSystem();

		const Uint32 RenderSystem::WindowType() {
			int context_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
			#ifdef StevEngine_DEBUGGING
			context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
			#endif
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4 );
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
			return SDL_WINDOW_OPENGL;
		}
		void RenderSystem::Init(SDL_Window* window) {
			//Create SDL OpenGL context
			context = SDL_GL_CreateContext(window);
			if (!context) {
				throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
			}
			SDL_GL_MakeCurrent(window, context);
			//Initialize GLAD:
			int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
			if (!version) {
				throw std::runtime_error("Failed to initialize GLAD");
			}
			Log::Debug(std::format("OpenGL Version: {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)), true);
			Log::Debug(std::format("OpenGL Shading Language Version: {}", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION)), true);
			Log::Debug(std::format("OpenGL Vendor: {}", (char *)glGetString(GL_VENDOR)), true);
			Log::Debug(std::format("OpenGL Renderer: {}", (char *)glGetString(GL_RENDERER)), true);
			//Enable GL options
			glEnable(GL_DEPTH_TEST);
			//Clear viewport
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
			//Buffers
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			// position layout
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Utilities::VERTEX_SIZE, (void*)0);
			glEnableVertexAttribArray(0);
			// uv layout
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Utilities::VERTEX_SIZE, (void*)((3) * sizeof(float)));
			glEnableVertexAttribArray(1);
			// Normal and Tangent layout
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Utilities::VERTEX_SIZE, (void*)((3 + 2) * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, Utilities::VERTEX_SIZE, (void*)((3 + 2 + 3) * sizeof(float)));
			glEnableVertexAttribArray(3);

			//Shaders
			ResetGlobalShader(VERTEX);
			ResetGlobalShader(FRAGMENT);

			//Set settings
			GameSettings gameSettings = engine->GetGameSettings();
			SetViewSize(gameSettings.WIDTH, gameSettings.HEIGHT);
			SetVSync(gameSettings.vsync);
			SetFaceCulling(true);
			if(gameSettings.MSAA == 0) Render::render.SetMSAA(false);
			else Render::render.SetMSAA(true, gameSettings.MSAA);
			SetAmbientLight(0.5, Utilities::Color(255));
			//Events
			engine->GetEvents()->Subscribe<WindowResizeEvent>([this] (WindowResizeEvent i) { return this->SetViewSize (i.width, i.height); });
			engine->GetEvents()->Subscribe<WindowVSyncEvent>([this] (WindowVSyncEvent i) { return this->SetVSync(i.value); });
			engine->GetEvents()->Subscribe<EngineDrawEvent>([this] (EngineDrawEvent) { return this->DrawFrame(); });

			Log::Debug("Renderer has been initialized!", true);
		}

		void RenderSystem::SetViewSize(int width, int height) {
			glViewport(0, 0, width, height);
		}

		void RenderSystem::SetVSync(bool vsync) {
			SDL_GL_SetSwapInterval(vsync);
		}

		void RenderSystem::SetFaceCulling(bool enable, GLenum face, bool clockwise) {
			if(enable) {
				glEnable(GL_CULL_FACE);
				glCullFace(face);
				glFrontFace(clockwise ? GL_CW : GL_CCW);
			} else {
				glDisable(GL_CULL_FACE);
			}
		}

		void RenderSystem::SetMSAA(bool enable, uint16_t amount) {
			if(enable) {
				glEnable(GL_MULTISAMPLE);
				if(amount < 2) return Log::Error("MultiSampling amount too small.", true);
				if((amount & (amount - 1)) != 0) return Log::Error("MultiSampling amount is not a power of 2.", true);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, amount);
				settings.Save("MSAA", amount);
			} else {
				glDisable(GL_MULTISAMPLE);
				settings.Save("MSAA", 0);
			}
			settings.SaveToFile();
		}


		void RenderSystem::ResetGlobalShader(ShaderType type) {
			if(type == VERTEX) {
				vertexShaderProgram = ShaderProgram(VERTEX);
				vertexShaderProgram.AddShader(Shader(vertexShaderSource, VERTEX));
				vertexShaderProgram.RelinkProgram();
			} else {
				fragmentShaderProgram = ShaderProgram(FRAGMENT);
				fragmentShaderProgram.AddShader(Shader(fragmentShaderSource, FRAGMENT));
				fragmentShaderProgram.RelinkProgram();
			}
			glDeleteProgramPipelines(1, &shaderPipeline);
			glGenProgramPipelines(1, &shaderPipeline);
			glBindProgramPipeline(shaderPipeline);
			glUseProgramStages(shaderPipeline, GL_VERTEX_SHADER_BIT, vertexShaderProgram.GetLocation());
			glUseProgramStages(shaderPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgram.GetLocation());
		}

		void RenderSystem::AddGlobalShader(ShaderProgram shader) {
			shader.RelinkProgram();
			if(shader.GetType() == VERTEX) {
				glDeleteProgram(vertexShaderProgram.GetLocation());
				vertexShaderProgram = shader;
			}
			else {
				glDeleteProgram(fragmentShaderProgram.GetLocation());
				fragmentShaderProgram = shader;
			}
			glDeleteProgramPipelines(1, &shaderPipeline);
			glGenProgramPipelines(1, &shaderPipeline);
			glBindProgramPipeline(shaderPipeline);
			glUseProgramStages(shaderPipeline, GL_VERTEX_SHADER_BIT, vertexShaderProgram.GetLocation());
			glUseProgramStages(shaderPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgram.GetLocation());
		}

		void RenderSystem::DrawObject(const CustomObject& object, glm::mat4x4 transform, RenderQueue queue) {
			queues[queue].emplace_back(object, transform);
		};

		void RenderSystem::DrawFrame() {
			//Clear color and depth buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Bind vertex array
			glBindVertexArray(VAO);

			//Camera matrices
			Visuals::Camera* camera = sceneManager.GetActiveScene()->GetCamera();
			//  View matrix
			vertexShaderProgram.SetShaderUniform("viewTransform", camera->GetView());
			fragmentShaderProgram.SetShaderUniform("viewPosition", (glm::vec3)camera->GetParent()->GetWorldPosition());
			//  Projection matrix
			vertexShaderProgram.SetShaderUniform("projectionTransform", camera->GetProjection());
			//Lights
			for(Light* light : lights) {
				light->UpdateShader(fragmentShaderProgram);
			}

			//Set background color
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

			//Render objects from each render queue
			for(int i = 0; i < queues.size(); i++) {
				//Enable depth masking?
				if(i == RenderQueue::TRANSPARENT) glDepthMask(GL_FALSE);
				else glDepthMask(GL_TRUE);
				//Draw objects
				for(RenderObject& object : queues[i]) {
					object.Draw();
				}
				//Clear queue
				queues[i].clear();
			}

			//Cleanup
			glBindVertexArray(0);
			// Refresh OpenGL window
			SDL_GL_SwapWindow(engine->window);
		}

		void RenderSystem::SetBackground(const Utilities::Color& color) {
			backgroundColor = color;
		}

		void RenderSystem::SetAmbientLight(float strength, const Utilities::Color& color) {
			ambientLightColor = color;
			ambientLightStrength = strength;
			fragmentShaderProgram.SetShaderUniform("ambientColor", glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f));
			fragmentShaderProgram.SetShaderUniform("ambientStrength", strength);
		}

		uint32_t RenderSystem::GetLightID(std::string type) {
			uint32_t next = 0;
			for(Light* light : lights) {
				if(light->type == type) {
					if(next == light->shaderLightID) {
						next++;
					} else {
						break;
					}
				}
			}
			return next;
		}
	}
}
#endif