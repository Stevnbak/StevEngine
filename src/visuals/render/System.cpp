#ifdef StevEngine_RENDERER_GL
#include "System.hpp"

#include "main/Log.hpp"
#include "main/Engine.hpp"
#include "visuals/render/Object.hpp"
#include "visuals/render/Lights.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp>

using StevEngine::Utilities::Vertex;

namespace StevEngine {
    namespace Render {
        const char* vertexShaderSource =
            #include "visuals/shaders/default.vert"
        ;
        const char* fragmentShaderSource =
            #include "visuals/shaders/default.frag"
        ;

        System::System() {}
        void System::Init() {
            glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
            //Buffers
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // position layout
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::size, (void*)0);
            glEnableVertexAttribArray(0);
            // normal layout
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::size, (void*)((3) * sizeof(float)));
            glEnableVertexAttribArray(1);
            // uv layout
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::size, (void*)((3 + 3) * sizeof(float)));
            glEnableVertexAttribArray(2);
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
                Log::Error("Vertex shader failed to compile!\n" + std::string(infoLog));
            }

            unsigned int fragmentShader;
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                Log::Error("Fragment shader failed to compile!\n" + std::string(infoLog));
            }

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                Log::Error("Shader program failed to compile!\n" + std::string(infoLog));
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            Log::Debug("Renderer has been initialized!", true);
        }

        void System::DrawObject(Object object, glm::mat4x4 transform, RenderQueue queue) {
            queues[queue].push_back({object, transform});
        };

        void System::DrawFrame() {
            //Use default shader program
            glUseProgram(shaderProgram);

            //Bind vertex array
            glBindVertexArray(VAO);

            //Camera matrices
            Visuals::Camera* camera = Engine::Instance->scenes.GetActiveScene()->GetCamera();
            //  View matrix
            glm::mat4x4 view = camera->GetView();
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewPosition"), 1, GL_FALSE, glm::value_ptr((glm::vec3)camera->GetParent()->GetWorldPosition()));
            //  Projection matrix
            glm::mat4x4 proj = camera->GetProjection();
            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

            //Lights
            for(Light* light : lights) {
                light->UpdateShader();
            }

            //Set background color
            glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

            //Draw objects
            for(int i = 0; i < queues.size(); i++) {
                for(RenderObject object : queues[i]) {
                    Draw(object);
                }
                queues[i].clear();
            }
            
            //Cleanup
            glBindVertexArray(0);
        }

        void System::SetBackground(SDL_Color color) {
            backgroundColor = color;
        }

        void System::SetAmbientLight(float strength, SDL_Color color) {
            glUniform3fv(glGetUniformLocation(shaderProgram, "ambientColor"), 1, glm::value_ptr(glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f)));
            glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), strength);
        }

        void System::Draw(RenderObject renderObject) {
            Object object = renderObject.object;
            glm::mat4x4 transform = renderObject.transform;
            //Update transform
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "objectTransform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            //Update texture
            unsigned int isTexturedLoc = glGetUniformLocation(shaderProgram, "objectIsTextured");
            glUniform1i(isTexturedLoc, (int)object.textured);
            if(object.textured) {
                unsigned int textureLoc = glGetUniformLocation(shaderProgram, "objectTexture");
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, object.texture);
                glUniform1i(textureLoc, 0);
            }
            //Update color
            unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
            glUniform4fv(colorLoc, 1, glm::value_ptr(glm::vec4(object.color.r / 255.0f, object.color.g / 255.0f, object.color.b / 255.0f, object.color.a / 255.0f)));
            //Update material
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectMaterial.ambient"), 1, glm::value_ptr((glm::vec3)object.material.ambient));
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectMaterial.diffuse"), 1, glm::value_ptr((glm::vec3)object.material.diffuse));
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectMaterial.specular"), 1, glm::value_ptr((glm::vec3)object.material.specular));
            glUniform1f(glGetUniformLocation(shaderProgram, "objectMaterial.shininess"), object.material.shininess);
            //Draw object
            glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(float), object.vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indices.size() * sizeof(float), object.indices.data(), GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, 0);
        }

        unsigned int System::GetLightID(std::string type) {
            unsigned int next = 0;
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
