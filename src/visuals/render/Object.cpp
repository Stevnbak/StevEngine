#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "utilities/Texture.hpp"

#include "glad/glad.h"
#include <algorithm>

using StevEngine::Utilities::Vertex;
using StevEngine::Utilities::Color;

namespace StevEngine {
    namespace Render {
        std::vector<float> ToFloatList(std::vector<Vertex> vertices) {
            std::vector<float> result;
            result.resize(vertices.size() * (3+3+2));
            int j = 0;
            for(int i = 0; i < vertices.size(); i++) {
                Vertex vertex = vertices[i];
                result[j++] = vertex.x;
                result[j++] = vertex.y;
                result[j++] = vertex.z;
                result[j++] = vertex.nX;
                result[j++] = vertex.nY;
                result[j++] = vertex.nZ;
                result[j++] = vertex.u;
                result[j++] = vertex.v;
            }
            return result;
        }
        Object::Object(std::vector<Vertex> vertices, Color color, Utilities::Texture textureData) {
            //Create indices and filter out duplicates
            std::vector<Vertex> uniqueVertices;
            for (Vertex v : vertices) {
                int index = std::find(uniqueVertices.begin(), uniqueVertices.end(), v) - uniqueVertices.begin();
                if(index == uniqueVertices.size()) {
                    uniqueVertices.push_back(v);
                }
                indices.push_back(index);
            }
            //Generate vertex array
            this->vertices = ToFloatList(uniqueVertices);
            //Bind texture
            SetTexture(textureData);
        }
        Object::Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Color color, Utilities::Texture textureData) : vertices(ToFloatList(vertices)), indices(indices) {
            SetTexture(textureData);
        }
        void Object::SetTexture(Utilities::Texture textureData) {
            texture = textureData.BindTexture();
            if(texture) {
                textured = true;
                
            } else {
                textured = false;
            }
        }
        void Object::FreeTexture() {
            glDeleteTextures(1, &texture);
            texture = 0;
        }
    }
}
#endif
