#include "Object.hpp"

#include "glad/glad.h"

namespace StevEngine {
    namespace Render {
        GLsizei Vertex::size = (3 + 2) * sizeof(float);
        bool Vertex::operator== (const Vertex o) const {
            return 
                (o.x == x) &&
                (o.y == y) &&
                (o.z == z) &&
                (o.texX == texX) &&
                (o.texY == texY);
        }

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
    }
}