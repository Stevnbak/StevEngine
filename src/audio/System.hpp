#pragma once
#include "audio/Emitter.hpp"

#include "vector"

namespace StevEngine::Audio {
    class System {
        public:
            System();
            void Play(Emitter* emitter);
            void CleanUp();
        private:
            int audio_rate;
            Uint16 audio_format;
            int audio_channels;
            int loops;
            int audio_open = 0;
    };
}