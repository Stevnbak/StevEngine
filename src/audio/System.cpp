#include <SDL_audio.h>
#include <cstddef>
#include <format>
#include <string>
#include <vector>
#ifdef StevEngine_AUDIO
#include "System.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "main/Engine.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

#include <algorithm>
#include <stdexcept>

static void SDLCALL StaticChannelCompleted (int channel)
{
	StevEngine::Engine::Instance->audio.ChannelCompleted(channel);
}

namespace StevEngine::Audio {
	System::System() {
		volumeSounds = 1.0;
		volumeMusic = 1.0;
		//Initialize
		activeSounds.push_back(nullptr);
		audioRate = MIX_DEFAULT_FREQUENCY;
		audioFormat = MIX_DEFAULT_FORMAT;
		audioChannels = MIX_DEFAULT_CHANNELS;
		audioDevice = NULL;
		// Open the audio device
		std::string devices = "";
		for(const char* device : GetAudioDevices()) devices += std::string(device) + ", ";
		Log::Debug("All audio devices: " + devices);
		SetAudioDevice(NULL);
		//Set channel finished callback
		Mix_ChannelFinished(StaticChannelCompleted);
	}

	void SDLCALL System::ChannelCompleted (int channel)
	{
		for(int i = 1; i < activeSounds.size(); i++) {
			if(activeSounds[i]->channel == channel) {
				activeSounds[i]->channel = -1;
				activeSounds.erase(activeSounds.begin() + i);
			}
		}
	}

	void System::Play(Emitter* emitter) {
		if(!audioOpen) return;
		if(emitter->channel != -1) {
			Mix_HaltChannel(emitter->channel);
		}
		int channel = Mix_PlayChannel(-1, emitter->GetData(), emitter->loop ? -1 : 0);
		Mix_Volume(channel, emitter->volume * volumeSounds * MIX_MAX_VOLUME);
		emitter->channel = channel;
		if(channel != -1) {
			activeSounds.push_back(emitter);
		}
	}

	void System::PlayBackground(std::string path, bool loop, double volume) {
		if(!audioOpen) return;
		if(music != NULL) {
			Mix_FreeMusic(music);
			music = NULL;
		}
		SDL_RWops* data = Engine::Instance->resources.GetFile(path).GetSDLData();
		music = Mix_LoadMUS_RW(data, 1);
		if (music == NULL) {
			Log::Error(std::format("Couldn't load {}: {}", path, SDL_GetError()), true);
		} else {
			Mix_PlayMusic(music, loop ? -1 : 0);
			Mix_VolumeMusic(volume * volumeMusic * MIX_MAX_VOLUME);
		}
	}

	void System::Stop(int channel) {
		if(!audioOpen) return;
		Mix_HaltChannel(channel);
	}

	void System::StopBackground() {
		if(!audioOpen) return;
		Mix_FreeMusic(music);
	}

	void System::SetSoundsVolume(double volume) {
		volumeSounds = volume;
		Engine::Instance->settings.Save("audio.soundVolume", volume);
	}

	void System::SetMusicVolume(double volume) {
		volumeMusic = volume;
		Engine::Instance->settings.Save("audio.musicVolume", volume);
	}

	std::vector<const char*> System::GetAudioDevices() {
		std::vector<const char*> devices;
		int count = SDL_GetNumAudioDevices(0);
		if(count < 0) return devices;
		devices.resize(count);
		for(int i = 0; i < count; i++) {
			devices[i] = SDL_GetAudioDeviceName(i, 0);
		}
		return devices;
	}

	void System::SetAudioDevice(const char *device) {
		CleanUp();
		audioDevice = device;
		if(device != NULL) Engine::Instance->settings.Save("audio.device", std::string(device));
		if (Mix_OpenAudioDevice(audioRate, audioFormat, audioChannels, 4096, audioDevice, SDL_AUDIO_ALLOW_CHANNELS_CHANGE | SDL_AUDIO_ALLOW_FREQUENCY_CHANGE) < 0) {
			throw std::runtime_error(std::format("Couldn't open audio device: %s\n", SDL_GetError()));
			CleanUp();
		} else {
			Mix_QuerySpec(&audioRate, &audioFormat, &audioChannels);
			Log::Debug(std::format("Opened audio at {} Hz {} bit{} {}",
				audioRate,
				(audioFormat&0xFF),
				(SDL_AUDIO_ISFLOAT(audioFormat) ? " (float)" : ""),
				(audioChannels > 2) ? "surround" :
				(audioChannels > 1) ? "stereo" : "mono"), true);
			audioOpen = true;
		}
	}

	void System::CleanUp() {
		if (audioOpen) {
			Mix_CloseAudio();
			audioOpen = false;
		}
	}
}
#endif
