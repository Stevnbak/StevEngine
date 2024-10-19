#pragma once
#ifdef StevEngine_AUDIO
#include "audio/Emitter.hpp"
#include <SDL.h>
#include <vector>
#include <cstdint>

namespace StevEngine::Audio {
	class AudioSystem {
		public:
			void Init();
			void Play(Emitter* emitter);
			void PlayBackground(std::string path, bool loop = true, double volume = 1);
			void Stop(int channel);
			void StopBackground();
			void SDLCALL ChannelCompleted(int channel);
			//Settings
			void SetSoundsVolume(double volume);
			double GetSoundsVolume() const { return volumeSounds; }
			void SetMusicVolume(double volume);
			double GetMusicVolume() const { return volumeMusic; }
			std::vector<const char*> GetAudioDevices();
			void SetAudioDevice(const char* device);
			const char* GetActiveAudioDevice() const { return audioDevice; }
		private:
			void CleanUp();
			double volumeSounds;
			double volumeMusic;
			const char* audioDevice;
			int audioRate;
			uint16_t audioFormat;
			int audioChannels;
			bool audioOpen = false;
			std::vector<Emitter*> activeSounds;
			Mix_Music* music = NULL;
	};

	extern AudioSystem audio;
}
#endif
