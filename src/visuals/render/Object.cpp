#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"

#include "glad/glad.h"
#include <algorithm>

using StevEngine::Utilities::Vertex;

namespace StevEngine {
    namespace Render {
        Object::Object(std::vector<Vertex> vertices, SDL_Color color, SDL_Surface* textureData) : vertices(std::vector<float>()), indices(std::vector<unsigned int>()) {
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
            this->vertices.resize(uniqueVertices.size() * 5);
            int j = 0;
            for(int i = 0; i < uniqueVertices.size(); i++) {
                Vertex vertex = uniqueVertices[i];
                this->vertices[j++] = vertex.x;
                this->vertices[j++] = vertex.y;
                this->vertices[j++] = vertex.z;
                this->vertices[j++] = vertex.texX;
                this->vertices[j++] = vertex.texY;
            }

            //Bind texture
            if(textureData) {
                textured = true;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_3D, texture);
                //Genereate texture
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData->pixels);
                glGenerateMipmap(GL_TEXTURE_2D);
                //Free up the image data memory.
                SDL_FreeSurface(textureData);
            } else {
                textured = false;
            }
        }
        Object::Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, SDL_Color color, SDL_Surface* textureData) : vertices(std::vector<float>()), indices(std::vector<unsigned int>()) {
            //Generate vertex array
            this->vertices.resize(vertices.size() * 5);
            int j = 0;
            for(int i = 0; i < vertices.size(); i++) {
                Vertex vertex = vertices[i];
                this->vertices[j++] = vertex.x;
                this->vertices[j++] = vertex.y;
                this->vertices[j++] = vertex.z;
                this->vertices[j++] = vertex.texX;
                this->vertices[j++] = vertex.texY;
            }
            //Set indices
            this->indices = indices;
            //Bind texture
            if(textureData) {
                textured = true;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_3D, texture);
                //Genereate texture
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData->pixels);
                glGenerateMipmap(GL_TEXTURE_2D);
                //Free up the image data memory.
                SDL_FreeSurface(textureData);
            } else {
                textured = false;
            }
        }
    }
}
#endif
