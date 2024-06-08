#include "Emitter.hpp"
#include "core/Engine.hpp"
#include "audio/System.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

namespace StevEngine::Audio {
    Emitter::Emitter(std::string audioPath, bool loop, double volume) {
        //Set basic variables
        this->audioPath = audioPath;
        this->audioData = NULL;
        this->loop = loop;
        this->volume = volume;
        //Load audo file
        audioData = Mix_LoadWAV_RW(Engine::Instance->resources->GetFile(audioPath).data, 1);
        if (audioData == NULL) {
            if (audioData) {
                Mix_FreeChunk(audioData);
                audioData = NULL;
            }
            throw std::format("Couldn't load {}: {}\n", audioPath, SDL_GetError());
        }
    }

    Mix_Chunk* Emitter::GetData() {
        return audioData;
    }

    void Emitter::Play() {
        Engine::Instance->audio->Play(this);
    }

    void Emitter::Destroy() {
        if (audioData) {
            Mix_FreeChunk(audioData);
            audioData = NULL;
        }
    }
}