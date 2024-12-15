#pragma once
#ifdef StevEngine_AUDIO
#include "main/Component.hpp"
#include <SDL_mixer.h>

namespace StevEngine::Audio {
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
			Emitter(std::string audioPath, bool loop = false, double volume = 1);
			Emitter(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;
			void Play();
			void ChangeSource(std::string path);
			Mix_Chunk* GetData() const { return audioData; };
			bool isPlaying() const { return channel != -1; }
		private:
			void Deactivate();
			~Emitter();
	};
	inline bool emitter = CreateComponents::RegisterComponentType<Emitter>("Emitter");
}
#endif
