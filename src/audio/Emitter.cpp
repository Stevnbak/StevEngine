#ifdef StevEngine_AUDIO
#include "Emitter.hpp"
#include "audio/System.hpp"
#include "scenes/Component.hpp"
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
		ChangeSource(audioPath);
	}

	void Emitter::Play() {
		audio.Play(this);
	}

	void Emitter::ChangeSource(std::string path) {
		if(audioData != NULL) {
			Mix_FreeChunk(audioData);
			audioData = NULL;
		}
		audioPath = path;
		SDL_RWops* data = Resources::resourceManager.GetFile(audioPath).GetSDLData();
		audioData = Mix_LoadWAV_RW(data, 0);
		SDL_FreeRW(data);
		if (audioData == NULL) {
			Log::Error(std::format("Couldn't load {}: {}", audioPath, SDL_GetError()), true);
		}
	}

	void Emitter::Deactivate() {
		if(channel != -1) audio.Stop(channel);
	}

	Emitter::Emitter(YAML::Node node) : Component(node) {
		//Set basic variables
		audioPath = node["file"].as<std::string>();
		audioData = NULL;
		loop = node["loop"].as<bool>();
		volume = node["volume"].as<double>();
		channel = -1;
		//Load audo file
		ChangeSource(audioPath);
	}
	YAML::Node Emitter::Export(YAML::Node node) const {
		node["file"] = audioPath;
		node["loop"] = loop;
		node["volume"] = volume;
		return node;
	}

	Emitter::~Emitter() {
		if (audioData) {
			Mix_FreeChunk(audioData);
			audioData = NULL;
		}
	}
}
#endif
