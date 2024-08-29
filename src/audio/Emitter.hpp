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
				Emitter(std::string audioPath, bool loop = false, double volume = 1);
				Emitter(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
				void Play();
				void ChangeSource(std::string path);
				Mix_Chunk* GetData() { return audioData; };
				bool isPlaying() { return channel != -1; }
			private:
				void Start() {};
				void Deactivate();
				void Update(double deltaTime) {};
				void Draw(glm::mat4x4 transform) {};
				~Emitter();
		};
		inline bool emitter = CreateComponents::RegisterComponentType<Emitter>("Emitter");
	}
}
#endif
