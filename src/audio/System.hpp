#pragma once
#include "audio/Emitter.hpp"
#include <SDL.h>
#include "vector"

namespace StevEngine {
    class Engine;
    namespace Audio {
        class System {
            friend class StevEngine::Engine;
            public:
                void Play(Emitter* emitter);
                void SDLCALL ChannelCompleted(int channel);
            private:
                System();
                void CleanUp();
                int audio_rate;
                Uint16 audio_format;
                int audio_channels;
                int audio_open = 0;
                std::vector<Emitter*> activeSounds;
        };
    }
}