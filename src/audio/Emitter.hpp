#pragma once
#include "core/GameObject.hpp"
#include <SDL_mixer.h>

namespace StevEngine::Audio {
    class Emitter : public Component {
        public:
            void Start() {};
            void Update(double deltaTime) {};
			void Draw() {};
            void Export(tinyxml2::XMLElement* element);
            Emitter(tinyxml2::XMLElement* node);
            Emitter(std::string audioPath, bool loop = false, double volume = 1);
            bool loop;
            double volume;
            void Play();
            void Destroy();
            Mix_Chunk* GetData() { return audioData; };
            int channel;
            bool isPlaying() { return channel != -1; }
        private:
            std::string audioPath;
            Mix_Chunk* audioData;
    };
}