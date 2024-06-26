#include "System.hpp"

#include "main/Log.hpp"
#include "main/Engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace StevEngine {
    namespace Render {
        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "out vec4 outColor;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
            "   outColor = aColor;\n"
            "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec4 outColor;\n"
            "void main()\n"
            "{\n"
                "FragColor = outColor;\n"
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
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3* sizeof(float)));
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

        void System::DrawObject(Object object) {
            //Update transform
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(object.transform));
            //Create vertices & indices arrays
            float vertices[(int)(object.vertices.size() * (7.0/3.0))];
            int j = 0;
            int t=object.vertices.size();
            for(int i = 0; i < object.vertices.size(); i += 3) {
                vertices[j++] = object.vertices[i];
                vertices[j++] = object.vertices[i+1];
                vertices[j++] = object.vertices[i+2];
                vertices[j++] = object.color.r;
                vertices[j++] = object.color.g;
                vertices[j++] = object.color.b;
                vertices[j++] = object.color.a;
            }
            unsigned int indices[object.indices.size()];
            std::copy(object.indices.begin(), object.indices.end(), indices);
            //Draw object
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, 0);
        }
        
        void System::StartFrame() {
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Temp

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            
            Camera* camera = Engine::Instance->scenes.GetActiveScene()->GetCamera();
            //View matrix
            glm::mat4x4 view = camera->GetView();
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            //Projection matrix
            glm::mat4x4 proj = camera->GetProjection();
            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        }

        void System::EndFrame() {
            glBindVertexArray(0);
            //objects.clear();
        }
    }
}