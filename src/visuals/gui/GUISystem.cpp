#include "inputs/InputSystem.hpp"
#include "main/EventSystem.hpp"
#include "utilities/Matrix4.hpp"
#include <SDL_keycode.h>
#ifdef StevEngine_GUI
#define NK_IMPLEMENTATION
#include "GUISystem.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "utilities/Matrix4.hpp"
#include "visuals/shaders/Shader.hpp"

#include "glad/gl.h"
#include <cstdint>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#define NK_WINDOW_OPTIONS NK_WINDOW_NO_SCROLLBAR

namespace StevEngine::Visuals {
	struct UI_Vertex
	{
		float position[2];
		float uv[2];
		nk_byte col[4];
	};
	GUISystem::GUISystem() {}

	void GUISystem::Init() {
		const GameSettings& settings = engine->GetGameSettings();
		nk_buffer_init_default(&cmds);
		//Create shaders
		shaderProgram = Renderer::ShaderProgram(Renderer::VERTEX, false);
		static const GLchar* vertex_shader =
			"#version 440\n"
			"uniform mat4 ProjMtx;\n"
			"layout(location = 0) in vec2 Position;\n"
			"layout(location = 1) in vec2 TexCoord;\n"
			"layout(location = 2) in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"out gl_PerVertex { vec4 gl_Position; };\n"
			"void main() {\n"
			"   Frag_UV = TexCoord;\n"
			"   Frag_Color = Color;\n"
			"   gl_Position = ProjMtx * vec4(Position, 0, 1);\n"
			"}\n";
		shaderProgram.AddShader(Renderer::Shader(vertex_shader, Renderer::VERTEX));
		static const GLchar* fragment_shader =
			"#version 440\n"
			"uniform sampler2D Texture;\n"
			"uniform bool usingTexture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main(){\n"
			"	vec4 tex = vec4(1.0);\n"
			"	if(usingTexture) { tex = texture(Texture, Frag_UV); }\n"
			"   Out_Color = Frag_Color * tex;\n"
			"}\n";
		shaderProgram.AddShader(Renderer::Shader(fragment_shader, Renderer::FRAGMENT));
		shaderProgram.RelinkProgram();
		//Create font
		nk_font_atlas atlas;
		nk_font_atlas_init_default(&atlas);
		nk_font_atlas_begin(&atlas);
		font = nk_font_atlas_add_default(&atlas, 13, 0);
		int w, h;
		const void* image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
		nk_draw_null_texture tex_null;
		glGenTextures(1, &font_texture);
		glBindTexture(GL_TEXTURE_2D, font_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)settings.WIDTH, (GLsizei)settings.HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		nk_font_atlas_end(&atlas, nk_handle_id((int)font_texture), &tex_null);
		//Init Nuklear
		int vs = sizeof(struct UI_Vertex);
		size_t vp = offsetof(struct UI_Vertex, position);
		size_t vt = offsetof(struct UI_Vertex, uv);
		size_t vc = offsetof(struct UI_Vertex, col);
		nk_init_default(&ctx, &font->handle);
		static const nk_draw_vertex_layout_element vertex_layout[] = {
			{NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct UI_Vertex, position)},
			{NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct UI_Vertex, uv)},
			{NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct UI_Vertex, col)},
			{NK_VERTEX_LAYOUT_END}
		};
		cfg.shape_AA = NK_ANTI_ALIASING_ON;
		cfg.line_AA = NK_ANTI_ALIASING_ON;
		cfg.vertex_layout = vertex_layout;
		cfg.vertex_size = vs;
		cfg.vertex_alignment = NK_ALIGNOF(struct UI_Vertex);
		cfg.circle_segment_count = 22;
		cfg.curve_segment_count = 22;
		cfg.arc_segment_count = 22;
		cfg.global_alpha = 1.0f;

		struct nk_rect rect = nk_rect(0, 0, settings.WIDTH, settings.HEIGHT);
		ctx.style.window.spacing = nk_vec2(0,0);
		ctx.style.window.padding = nk_vec2(0,0);
		ctx.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
		nk_begin(&ctx, "Canvas", rect, NK_WINDOW_OPTIONS);
		canvas = nk_window_get_canvas(&ctx);
		//Initialize buffers
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		// position layout
		glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, vp);
		glEnableVertexArrayAttrib(VAO, 0);
		glVertexArrayAttribBinding(VAO, 0, 0);
		// uv layout
		glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, vt);
		glEnableVertexArrayAttrib(VAO, 1);
		glVertexArrayAttribBinding(VAO, 1, 0);
		// color layout
		glVertexArrayAttribFormat(VAO, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, vc);
		glEnableVertexArrayAttrib(VAO, 2);
		glVertexArrayAttribBinding(VAO, 2, 0);

		glVertexArrayElementBuffer(VAO, EBO);
		glVertexArrayVertexBuffer(VAO, 0, VBO, 0, vs);
		//Event listeners
		EventManager* engineEvents = engine->GetEvents();
		engineEvents->Subscribe<EngineDrawEvent>([this] (EngineDrawEvent) { return DrawCanvas(); });
		engineEvents->Subscribe<UpdateEvent>([this] (UpdateEvent e) { return Update(e.deltaTime); });
		engineEvents->Subscribe<WindowResizeEvent>([this] (WindowResizeEvent i) { return SetViewSize (i.width, i.height); });

		//Input events
		engineEvents->Subscribe<PreUpdateEvent>([this] (PreUpdateEvent) {
			nk_input_begin(&ctx);
		});
		#ifdef StevEngine_INPUTS
		EventManager* inputEvents = inputManager.GetEvents();
		//Keyboard inputs
		inputEvents->Subscribe<InputKeyDownEvent>([this](InputKeyDownEvent e) {
			HandleKeyInput(e.key, true);
		});
		inputEvents->Subscribe<InputKeyUpEvent>([this](InputKeyUpEvent e) {
			HandleKeyInput(e.key, false);
		});
		//Mouse inputs
		inputEvents->Subscribe<InputMouseButtonDownEvent>([this](InputMouseButtonDownEvent e) {
			HandleMouseButtonInput(e.button, e.x, e.y, e.clicks, true);
		});
		inputEvents->Subscribe<InputMouseButtonUpEvent>([this](InputMouseButtonUpEvent e) {
			HandleMouseButtonInput(e.button, e.x, e.y, e.clicks, false);
		});
		inputEvents->Subscribe<InputMouseMoveEvent>([this](InputMouseMoveEvent e) {
			nk_input_motion(&ctx, e.x, e.y);
		});
		inputEvents->Subscribe<InputMouseWheelEvent>([this](InputMouseWheelEvent e) {
			nk_input_scroll(&ctx, nk_vec2(e.x, e.y));
		});
		//TODO: Add text input event!!
		#endif
	}

	GUISystem::~GUISystem() {
		nk_buffer_free(&cmds);
		nk_end(&ctx);
		glDeleteTextures(1, &font_texture);
	}

	void GUISystem::SetViewSize(int width, int height) {
		canvas->clip = nk_rect(0, 0, width, height);
	}

	void GUISystem::Update(double deltaTime) {
		ctx.delta_time_seconds = deltaTime;
		nk_input_end(&ctx);
	}

	void GUISystem::HandleKeyInput(SDL_Keycode key, bool down) {
		const uint8_t* state = SDL_GetKeyboardState(0);
		switch(key)
		{
			case SDLK_RSHIFT: /* RSHIFT & LSHIFT share same routine */
			case SDLK_LSHIFT:		nk_input_key(&ctx, NK_KEY_SHIFT, down); break;
			case SDLK_DELETE:		nk_input_key(&ctx, NK_KEY_DEL, down); break;
			case SDLK_RETURN:		nk_input_key(&ctx, NK_KEY_ENTER, down); break;
			case SDLK_TAB:			nk_input_key(&ctx, NK_KEY_TAB, down); break;
			case SDLK_BACKSPACE:	nk_input_key(&ctx, NK_KEY_BACKSPACE, down); break;
			case SDLK_HOME:			nk_input_key(&ctx, NK_KEY_TEXT_START, down);
									nk_input_key(&ctx, NK_KEY_SCROLL_START, down); break;
			case SDLK_END:	   		nk_input_key(&ctx, NK_KEY_TEXT_END, down);
									nk_input_key(&ctx, NK_KEY_SCROLL_END, down); break;
			case SDLK_PAGEDOWN:  	nk_input_key(&ctx, NK_KEY_SCROLL_DOWN, down); break;
			case SDLK_PAGEUP:		nk_input_key(&ctx, NK_KEY_SCROLL_UP, down); break;
			case SDLK_z:		 	nk_input_key(&ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_r:		 	nk_input_key(&ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_c:		 	nk_input_key(&ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_v:		 	nk_input_key(&ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_x:		 	nk_input_key(&ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_b:		 	nk_input_key(&ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_e:		 	nk_input_key(&ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]); break;
			case SDLK_UP:			nk_input_key(&ctx, NK_KEY_UP, down); break;
			case SDLK_DOWN:	  		nk_input_key(&ctx, NK_KEY_DOWN, down); break;
			case SDLK_LEFT:
				if (state[SDL_SCANCODE_LCTRL])
					nk_input_key(&ctx, NK_KEY_TEXT_WORD_LEFT, down);
				else nk_input_key(&ctx, NK_KEY_LEFT, down);
				break;
			case SDLK_RIGHT:
				if (state[SDL_SCANCODE_LCTRL])
					nk_input_key(&ctx, NK_KEY_TEXT_WORD_RIGHT, down);
				else nk_input_key(&ctx, NK_KEY_RIGHT, down);
				break;
		}
	}
	void GUISystem::HandleMouseButtonInput(uint8_t button, int x, int y, uint8_t clicks, bool down) {
        switch(button)
        {
            case SDL_BUTTON_LEFT:
                if (clicks > 1) nk_input_button(&ctx, NK_BUTTON_DOUBLE, x, y, down);
                nk_input_button(&ctx, NK_BUTTON_LEFT, x, y, down);
                break;
            case SDL_BUTTON_MIDDLE:
            	nk_input_button(&ctx, NK_BUTTON_MIDDLE, x, y, down);
             	break;
            case SDL_BUTTON_RIGHT:
            	nk_input_button(&ctx, NK_BUTTON_RIGHT, x, y, down);
             	break;
        }
	}

	#ifdef StevEngine_RENDERER_GL
	void GUISystem::DrawCanvas() {
		const GameSettings& settings = engine->GetGameSettings();
		nk_end(&ctx);
		glUseProgram(shaderProgram.GetLocation());
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glActiveTexture(GL_TEXTURE0);
		//Setup buffers
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_MEMORY, NULL, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_MEMORY, NULL, GL_STATIC_DRAW);
		// Load Vertices & Elements into buffers
		void* vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		void* elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		nk_buffer ebuf, vbuf;
		nk_buffer_init_fixed(&vbuf, vertices, MAX_VERTEX_MEMORY);
		nk_buffer_init_fixed(&ebuf, elements, MAX_ELEMENT_MEMORY);
		nk_convert(&ctx, &cmds, &vbuf, &ebuf, &cfg);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		//Set program uniforms
		Utilities::Matrix4 orthoMatrix = Utilities::Matrix4({
			{2.0 / (double)settings.WIDTH, 0.0, 0.0, -1.0},
			{0.0,-2.0 / (double)settings.HEIGHT, 0.0, 1.0},
			{0.0, 0.0,-1.0, 0.0},
			{0.0,0.0, 0.0, 1.0}
		});
		shaderProgram.SetShaderUniform("Texture", 0);
		shaderProgram.SetShaderUniform("ProjMtx", orthoMatrix);
		//Draw all elements
		const nk_draw_command* cmd = 0;
		const nk_draw_index* offset = NULL;
		nk_draw_foreach(cmd, &ctx, &cmds)
		{
			if (!cmd->elem_count) continue;
			shaderProgram.SetShaderUniform("usingTexture", cmd->texture.ptr != nullptr);
			glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
			glScissor(
				(int)(cmd->clip_rect.x),
				(int)(settings.HEIGHT - (int)(cmd->clip_rect.y + cmd->clip_rect.h)),
				(int)(cmd->clip_rect.w),
				(int)(cmd->clip_rect.h)
			);
			glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
			offset += cmd->elem_count / 2;
		}
		//Clear elements
		nk_clear(&ctx);
		nk_buffer_clear(&cmds);
		nk_begin(&ctx, "Canvas", nk_rect(0, 0, settings.WIDTH, settings.HEIGHT), NK_WINDOW_OPTIONS);
		canvas = nk_window_get_canvas(&ctx);
		//Free buffers
		nk_buffer_free(&vbuf);
		nk_buffer_free(&ebuf);
		//Reset OpenGL state
		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_BLEND);
		glDisable(GL_SCISSOR_TEST);
	}
	#else
	void GUISystem::DrawCanvas() {
		throw "No enabled renderer implementation found!";
	}
	#endif

	GUISystem guiSystem = GUISystem();
}
#endif