#ifdef StevEngine_RENDERER_GL
#include "RenderSystem.hpp"
#include "main/Log.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "data/Settings.hpp"
#include "main/SceneManager.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/Lights.hpp"
#include "visuals/shaders/Shader.hpp"
#include "visuals/shaders/ShaderProgram.hpp"
#include "visuals/Camera.hpp"

#include <algorithm>
#include <glad/gl.h>

using namespace StevEngine::Visuals;
namespace StevEngine::Renderer {
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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR );
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR );
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
		if(gameSettings.MSAA == 0) Renderer::render.SetMSAA(false);
		else Renderer::render.SetMSAA(true, gameSettings.MSAA);
		SetAmbientLight(0.1, Utilities::Color(255));
		//Events
		engine->GetEvents().Subscribe<WindowResizeEvent>([this] (WindowResizeEvent i) { return this->SetViewSize (i.width, i.height); });
		engine->GetEvents().Subscribe<WindowVSyncEvent>([this] (WindowVSyncEvent i) { return this->SetVSync(i.value); });
		engine->GetEvents().Subscribe<EngineDrawEvent>([this] (EngineDrawEvent) { return this->DrawFrame(); });

		SetEnabled(true);
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
			Data::settings.Save("MSAA", amount);
		} else {
			glDisable(GL_MULTISAMPLE);
			Data::settings.Save("MSAA", 0);
		}
		Data::settings.SaveToFile();
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

	void RenderSystem::ResetGPUBuffers() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}

	void RenderSystem::DrawObject(const CustomObject& object, Utilities::Matrix4 transform, RenderQueue queue) {
		queues[queue].emplace_back(object, transform);
	};

	void RenderSystem::DrawFrame() {
		if(!enabled) return;
		glUseProgram(0);
		glBindProgramPipeline(render.GetShaderPipeline());
		//Clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Bind vertex array
		ResetGPUBuffers();

		//Camera matrices
		Visuals::Camera* camera = sceneManager.GetActiveScene().GetCamera();
		//  View matrix
		vertexShaderProgram.SetShaderUniform("viewTransform", camera->GetView());
		fragmentShaderProgram.SetShaderUniform("viewPosition", camera->GetParent().GetWorldPosition());
		fragmentShaderProgram.SetShaderUniform("viewDirection", camera->GetParent().GetWorldRotation().Forward());
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

		// Refresh OpenGL window
		SDL_GL_SwapWindow(engine->window);
	}

	void RenderSystem::SetEnabled(bool enabled) {
		this->enabled = enabled;
	}

	void RenderSystem::SetBackground(const Utilities::Color& color) {
		backgroundColor = color;
	}

	void RenderSystem::SetAmbientLight(float strength, const Utilities::Color& color) {
		ambientLightColor = color;
		ambientLightStrength = strength;
		fragmentShaderProgram.SetShaderUniform("ambientColor", color);
		fragmentShaderProgram.SetShaderUniform("ambientStrength", strength);
	}

	uint32_t RenderSystem::GetLightID(std::string type) {
		uint32_t next = 0;
		for(Visuals::Light* light : lights) {
			if(light->GetType() == type) {
				if(next == light->GetShaderLightID()) {
					next++;
				} else {
					break;
				}
			}
		}
		return next;
	}

	void RenderSystem::AddLight(Visuals::Light* light) {
		lights.push_back(light);
	}

	void RenderSystem::RemoveLight(Visuals::Light* light) {
		lights.erase(std::find(lights.begin(), lights.end(), light));
	}
}
#endif
