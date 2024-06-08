#pragma once
#include "audio/Emitter.hpp"

#include <SDL.h>
#include "vector"

namespace StevEngine::Audio {
    class System {
        public:
            System();
            void Play(Emitter* emitter);
            void CleanUp();
            void SDLCALL ChannelCompleted(int channel);
        private:
            int audio_rate;
            Uint16 audio_format;
            int audio_channels;
            int audio_open = 0;
            std::vector<Emitter*> activeSounds;
    };
}