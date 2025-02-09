#pragma once
#ifdef StevEngine_AUDIO
#include "utilities/Serializable.hpp"
#include "main/Component.hpp"
#include <SDL_mixer.h>

#define EMITTER_TYPE "Emitter"

namespace StevEngine::Audio {

	/**
	 * @brief Component for playing sound effects from game objects
	 *
	 * The Emitter component allows game objects to play audio files. It supports
	 * one-shot and looping sounds with configurable volume levels.
	 */
	class Emitter : public Component {
		friend class StevEngine::GameObject;
		public:
			/** @brief Whether the sound should loop when playing */
			bool loop;

			/** @brief Volume level from 0.0 to 1.0 */
			double volume;

			/** @brief Current audio channel ID, or -1 if not playing */
			int channel;

		private:
			/** @brief Path to the audio file */
			std::string audioPath;

			/** @brief Cached audio data */
			Mix_Chunk* audioData;

		public:
			/**
			 * @brief Create a new audio emitter
			 * @param audioPath Path to the audio file to play
			 * @param loop Whether the sound should loop
			 * @param volume Initial volume level from 0.0 to 1.0
			 */
			Emitter(std::string audioPath, bool loop = false, double volume = 1);

			/**
			 * @brief Create emitter from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			Emitter(Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return EMITTER_TYPE; }

			/**
			 * @brief Serialize component to a text stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Stream Export(StreamType type) const;

			/**
			 * @brief Play the sound
			 *
			 * Starts playing the loaded audio file. If the sound is already
			 * playing, it will be stopped and restarted.
			 */
			void Play();

			/**
			 * @brief Change the audio source file
			 * @param path Path to new audio file
			 *
			 * Loads a new audio file, freeing the previous one if loaded.
			 */
			void ChangeSource(std::string path);

			/**
			 * @brief Get the raw audio data
			 * @return Pointer to loaded audio chunk
			 */
			Mix_Chunk* GetData() const { return audioData; };

			/**
			 * @brief Check if sound is currently playing
			 * @return true if sound is playing, false otherwise
			 */
			bool isPlaying() const { return channel != -1; }

		private:
			/**
			 * @brief Stop playing when component is deactivated
			 */
			void Deactivate();

			/**
			 * @brief Clean up audio resources
			 */
			~Emitter();
	};

	/** Register Emitter as a component type */
	inline bool emitter = CreateComponents::RegisterComponentType<Emitter>(EMITTER_TYPE);
}
#endif