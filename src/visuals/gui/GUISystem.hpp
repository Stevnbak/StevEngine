#pragma once
#ifdef StevEngine_GUI
#include "visuals/shaders/ShaderProgram.hpp"
#include "utilities/Matrix4.hpp"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>
#include <SDL_video.h>

namespace StevEngine::Visuals {
	class GUISystem {
		public:
			GUISystem();
			void Init();
			void DrawCanvas();
			nk_command_buffer* GetCanvas() { return canvas; }
			nk_user_font* GetDefaultFont() const { return &font->handle; }
			~GUISystem();
		private:
			Renderer::ShaderProgram shaderProgram;
			uint32_t VBO; //Vertex Buffer Object
			uint32_t EBO; //Element Buffer Object
			uint32_t VAO; //Vertex Array Object
			nk_context ctx; //Nuklear context
			nk_convert_config cfg; //Nuklear config
			nk_buffer cmds; //Nuklear draw command buffer
			nk_font* font; //Default font
			unsigned int font_texture; //Font texture
			nk_command_buffer* canvas; //Nuklear canvas buffer
			//Functions
			void SetViewSize(int width, int height);
			void Update(double deltaTime);
	};

	extern GUISystem guiSystem;
}
#endif