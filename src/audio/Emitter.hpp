#pragma once
#ifdef StevEngine_AUDIO
#include "scenes/Component.hpp"
#include <SDL_mixer.h>

namespace StevEngine {
    namespace Audio {
        class Emitter : public Component {
            friend class StevEngine::GameObject;
            public:
                bool loop;
                double volume;
                int channel;
            private:
                std::string audioPath;
                Mix_Chunk* audioData;
            public:
                Emitter(tinyxml2::XMLElement* node);
                Emitter(std::string audioPath, bool loop = false, double volume = 1);
                void Play();
                void ChangeSource(std::string path);
                Mix_Chunk* GetData() { return audioData; };
                bool isPlaying() { return channel != -1; }
            private:
                void Export(tinyxml2::XMLElement* element);
                void Start() {};
                void Deactivate();
                void Update(double deltaTime) {};
                void Draw(glm::mat4x4 transform) {};
                ~Emitter();
        };
    }
}
#endif