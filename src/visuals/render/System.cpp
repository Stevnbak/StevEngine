#ifdef StevEngine_RENDERER_GL
#include "System.hpp"

#include "main/Log.hpp"
#include "main/Engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using StevEngine::Utilities::Vertex;

namespace StevEngine {
    namespace Render {
        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 vPos;\n"
            "layout (location = 1) in vec2 vTex;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "out vec2 fTexCoord;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection * view * model * vec4(vPos, 1.0);\n"
            "   fTexCoord = vTex;\n"
            "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec2 fTexCoord;\n"
            "uniform vec4 fColor;\n"
            "uniform bool fIsTextured;\n"
            "uniform sampler2D fTexture;\n"
            "void main()\n"
            "{\n"
                "vec4 tex = vec4(1.0, 1.0, 1.0, 1.0);"
                "if (fIsTextured) {"
                    "tex = texture(fTexture, fTexCoord);"
                "}"
                "FragColor = tex * fColor;\n"
            "}\n";

        System::System() {}
        void System::Init() {
            //Buffers
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // position
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::size, (void*)0);
            glEnableVertexAttribArray(0);
            // texture coordinates
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vertex::size, (void*)((3) * sizeof(float)));
            glEnableVertexAttribArray(1);
            //Shaders
            unsigned int vertexShader;
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            int  success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                Log::Error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
            }

            unsigned int fragmentShader;
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                Log::Error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
            }

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                Log::Error("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" + std::string(infoLog));
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        void System::DrawObject(Object object, glm::mat4x4 transform) {
            //Update transform
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            //Update texture
            unsigned int isTexturedLoc = glGetUniformLocation(shaderProgram, "fIsTextured");
            glUniform1i(isTexturedLoc, (int)object.textured);
            if(object.textured) {
                unsigned int textureLoc = glGetUniformLocation(shaderProgram, "fTexture");
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, object.texture);
                glUniform1i(textureLoc, 0);
            }
            //Update color
            unsigned int colorLoc = glGetUniformLocation(shaderProgram, "fColor");
            glUniform4fv(colorLoc, 1, glm::value_ptr(glm::vec4(object.color.r,object.color.g,object.color.b,object.color.a)));
            //Draw object
            glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(float), object.vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indices.size() * sizeof(float), object.indices.data(), GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, 0);
        }

        void System::DrawCustomObject(CustomObject* object, glm::mat4x4 transform) {
            glUseProgram(0);
            object->Draw(transform);
            glUseProgram(shaderProgram);
        }
        
        void System::StartFrame() {
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Temp
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            
            Visuals::Camera* camera = Engine::Instance->scenes.GetActiveScene()->GetCamera();
            //View matrix
            glm::mat4x4 view = camera->GetView();
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            //Projection matrix
            glm::mat4x4 proj = camera->GetProjection();
            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
            //Set background color
            glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        }

        void System::EndFrame() {
            glBindVertexArray(0);
        }

        void System::SetBackground(SDL_Color color) {
            backgroundColor = color;
        }
    }
}
#endif