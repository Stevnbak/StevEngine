#include <stdexcept>
#ifdef StevEngine_AUDIO
#include "System.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "main/Engine.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

#include <algorithm>

static void SDLCALL StaticChannelCompleted (int channel)
{
    StevEngine::Engine::Instance->audio.ChannelCompleted(channel);
}


namespace StevEngine::Audio {
    System::System() {
        //Initialize
        activeSounds.push_back(nullptr);
        audio_rate = MIX_DEFAULT_FREQUENCY;
        audio_format = MIX_DEFAULT_FORMAT;
        audio_channels = MIX_DEFAULT_CHANNELS;
        // Open the audio device
        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096) < 0) {
            throw std::runtime_error(std::format("Couldn't open audio: %s\n", SDL_GetError()));
            CleanUp();
        } else {
            Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
            Log::Debug(std::format("Opened audio at {} Hz {} bit{} {}", audio_rate,
                (audio_format&0xFF),
                (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : ""),
                (audio_channels > 2) ? "surround" :
                (audio_channels > 1) ? "stereo" : "mono"), true);
        }
        audio_open = 1;
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
        if(emitter->channel != -1) {
            Mix_HaltChannel(emitter->channel);
        }
        int channel = Mix_PlayChannel(-1, emitter->GetData(), emitter->loop ? -1 : 0);
        Mix_Volume(channel, emitter->volume * MIX_MAX_VOLUME);
        emitter->channel = channel;
        if(channel != -1) {
            activeSounds.push_back(emitter);
        }
    }

    void System::PlayBackground(std::string path, bool loop, double volume) {
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
            Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
        }
    }


    void System::Stop(int channel) {
        Mix_HaltChannel(channel);
    }

    void System::StopBackground() {
        Mix_FreeMusic(music);
    }

    void System::CleanUp() {
        if (audio_open) {
            Mix_CloseAudio();
            audio_open = 0;
        }
    }
}
#endif
