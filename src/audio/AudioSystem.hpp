#pragma once
#ifdef StevEngine_AUDIO
#include "audio/Emitter.hpp"
#include <SDL.h>
#include <vector>
#include <cstdint>

namespace StevEngine::Audio {
	/**
	 * @brief Main audio system for handling sound playback and management
	 *
	 * The AudioSystem manages all audio output including sound effects and music.
	 * It handles device selection, volume control, and playback of both one-shot
	 * and looping sounds through SDL_mixer.
	 */
	class AudioSystem {
		public:
			/**
			 * @brief Initialize the audio system
			 *
			 * Sets up SDL_mixer, loads available audio devices, and initializes
			 * default volume levels and audio format settings.
			 */
			void Init();

			/**
			 * @brief Play an audio emitter
			 * @param emitter The audio emitter component to play
			 *
			 * Plays the sound associated with the given emitter. If the emitter
			 * is already playing, it will be stopped and restarted.
			 */
			void Play(Emitter* emitter);

			/**
			 * @brief Play background music
			 * @param path Path to the audio file
			 * @param loop Whether to loop the music
			 * @param volume Volume level from 0.0 to 1.0
			 *
			 * Loads and plays music from the specified file. Previous music
			 * will be stopped automatically.
			 */
			void PlayBackground(std::string path, bool loop = true, double volume = 1);

			/**
			 * @brief Stop audio on a specific channel
			 * @param channel The channel ID to stop
			 */
			void Stop(int channel);

			/**
			 * @brief Stop currently playing background music
			 */
			void StopBackground();

			/**
			 * @brief Channel finished callback
			 * @param channel The channel that finished playing
			 *
			 * Called by SDL_mixer when a sound finishes playing
			 */
			void SDLCALL ChannelCompleted(int channel);

			/**
			 * @brief Set volume for sound effects
			 * @param volume Volume level from 0.0 to 1.0
			 *
			 * Sets the master volume level for all sound effects.
			 * Volume changes are saved to settings.
			 */
			void SetSoundsVolume(double volume);
			double GetSoundsVolume() const { return volumeSounds; }

			/**
			 * @brief Set volume for background music
			 * @param volume Volume level from 0.0 to 1.0
			 *
			 * Sets the master volume level for background music.
			 * Volume changes are saved to settings.
			 */
			void SetMusicVolume(double volume);
			double GetMusicVolume() const { return volumeMusic; }

			/**
			 * @brief Get list of available audio output devices
			 * @return Vector of audio device names
			 */
			std::vector<const char*> GetAudioDevices();

			/**
			 * @brief Set active audio output device
			 * @param device Name of audio device to use, or NULL for default
			 *
			 * Changes the audio output device. The current device will be
			 * closed and audio reinitialized with the new device.
			 */
			void SetAudioDevice(const char* device);
			const char* GetActiveAudioDevice() const { return audioDevice; }

		private:
			/**
			 * @brief Clean up audio system resources
			 */
			void CleanUp();

			double volumeSounds;	  ///< Master volume for sound effects
			double volumeMusic;	   ///< Master volume for music
			const char* audioDevice;  ///< Currently selected audio device
			int audioRate;		   ///< Sample rate (Hz)
			uint16_t audioFormat;	///< Audio format
			int audioChannels;	   ///< Number of audio channels
			bool audioOpen = false;  ///< Whether audio device is open
			std::vector<Emitter*> activeSounds;  ///< Currently playing sounds
			Mix_Music* music = NULL; ///< Current background music
	};

	/** Global audio system instance */
	extern AudioSystem audio;
}
#endif