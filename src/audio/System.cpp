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
        ///activeSounds = std::vector<Emitter*>();
        activeSounds.push_back(nullptr);
        audio_rate = MIX_DEFAULT_FREQUENCY;
        audio_format = MIX_DEFAULT_FORMAT;
        audio_channels = MIX_DEFAULT_CHANNELS;
        // Open the audio device
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
        }
        audio_open = 1;
        //Set channel finished callback
        Mix_ChannelFinished(StaticChannelCompleted);
    }

    void SDLCALL System::ChannelCompleted (int channel)
    {
        Log::Normal("Channel completed");
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
        emitter->channel = channel;
        if(channel != -1) {
            activeSounds.push_back(emitter);
        }
    }

    void System::Stop(int channel) {
        Mix_HaltChannel(channel);
    }

    void System::CleanUp() {
        if (audio_open) {
            Mix_CloseAudio();
            audio_open = 0;
        }
    }
}