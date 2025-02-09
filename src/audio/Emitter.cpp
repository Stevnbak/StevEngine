#ifdef StevEngine_AUDIO
#include "Emitter.hpp"
#include "audio/AudioSystem.hpp"
#include "main/ResourceManager.hpp"
#include "main/Log.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

namespace StevEngine::Audio {
	Emitter::Emitter(std::string audioPath, bool loop, double volume) {
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

	Emitter::Emitter(Stream& stream) {
		//Set basic variables
		stream >> audioPath >> loop >> volume;
		audioData = NULL;
		channel = -1;
		//Load audio file
		ChangeSource(audioPath);
	}
	Stream Emitter::Export(StreamType type) const {
		Stream stream(type);
		stream << audioPath << loop << volume;
		return stream;
	}
	Emitter::~Emitter() {
		if (audioData) {
			Mix_FreeChunk(audioData);
			audioData = NULL;
		}
	}
}
#endif
