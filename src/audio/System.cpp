#include "System.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

#include "core/Log.hpp"
#include "core/ResourceManager.hpp"
#include "core/Engine.hpp"


namespace StevEngine::Audio {
    System::System() {
        //Initialize
        audio_rate = MIX_DEFAULT_FREQUENCY;
        audio_format = MIX_DEFAULT_FORMAT;
        audio_channels = MIX_DEFAULT_CHANNELS;
        loops = 0;

        /* Open the audio device */
        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096) < 0) {
            throw std::format("Couldn't open audio: %s\n", SDL_GetError());
            CleanUp();
        } else {
            Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
            Log::Normal(std::format("Opened audio at {} Hz {} bit{} {}", audio_rate,
                (audio_format&0xFF),
                (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : ""),
                (audio_channels > 2) ? "surround" :
                (audio_channels > 1) ? "stereo" : "mono"));
            if (loops) {
                Log::Normal(" (looping)\n");
            } else {
                putchar('\n');
            }
        }
        audio_open = 1;
    }

    void System::Play(Emitter* emitter) {
        Mix_PlayChannel(-1, emitter->GetData(), emitter->loop ? -1 : 0);
    }

    void System::CleanUp() {
        if (audio_open) {
            Mix_CloseAudio();
            audio_open = 0;
        }
    }
}