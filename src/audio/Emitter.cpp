#include "Emitter.hpp"
#include "audio/System.hpp"
#include "main/Engine.hpp"
#include "scenes/GameObject.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

namespace StevEngine::Audio {
    Emitter::Emitter(std::string audioPath, bool loop, double volume): Component("Emitter") {
        //Set basic variables
        this->audioPath = audioPath;
        this->audioData = NULL;
        this->loop = loop;
        this->volume = volume;
        this->channel = -1;
        //Load audo file
        SDL_RWops* data = Engine::Instance->resources.GetFile(audioPath).GetSDLData();
        audioData = Mix_LoadWAV_RW(data, 0);
        SDL_FreeRW(data);
        if (audioData == NULL) {
            Log::Error(std::format("Couldn't load {}: {}", audioPath, SDL_GetError()), true);
        }
    }

    void Emitter::Play() {
        Engine::Instance->audio.Play(this);
    }

    void Emitter::Deactivate() {
        if(channel != -1) Engine::Instance->audio.Stop(channel);
    }

    void Emitter::Export(tinyxml2::XMLElement* element) {
        element->SetAttribute("path", audioPath.c_str());
        element->SetAttribute("loop", loop);
        element->SetAttribute("volume", volume);
    }
    Emitter::Emitter(tinyxml2::XMLElement* node) : Emitter(node->Attribute("path"), node->BoolAttribute("loop"), node->DoubleAttribute("volume")) {}
	FactoryBase* factory = GameObject::AddComponentFactory<Emitter>(std::string("Emitter"));


    Emitter::~Emitter() {
        if (audioData) {
            Mix_FreeChunk(audioData);
            audioData = NULL;
        }
    }
}