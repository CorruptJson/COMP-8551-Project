#include "Sound.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

Sound::Sound() 
{
    SDL_Init(SDL_INIT_AUDIO);
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step one: Couldn't init audio: %s", Mix_GetError());
        exit(-1);
    }

}

Sound::~Sound()
{
    for (Mix_Chunk* p : mSFXBank)
        Mix_FreeChunk(p);

    for (Mix_Music* p : mMusicBank)
        Mix_FreeMusic(p);

    mSFXBank.clear();
    mMusicBank.clear();

    SDL_Quit();
}

void Sound::loadSfx(std::vector<std::string> sfxPaths)
{
    for (std::string n : sfxPaths) {
        Mix_Chunk* sfxMix = Mix_LoadWAV(n.c_str());
        
        if (sfxMix != nullptr)
            mSFXBank.push_back(sfxMix);
        else
            std::cerr << "Failed to load " << n << std::endl;
    }
}

void Sound::loadMusic(std::vector<std::string> musicPaths)
{
    for (std::string n : musicPaths) {
        Mix_Music* musicChunk = Mix_LoadMUS(n.c_str());

        if (musicChunk != nullptr)
            mMusicBank.push_back(musicChunk);
        else
            std::cerr << "Failed to load " << n << std::endl;
    }
}

void Sound::playSound(int index)
{
    if (mSFXBank.size() == 0)
        return;

    if (mSFXBank[index] != nullptr)
    {
        Mix_PlayChannel(-1, mSFXBank[index], 0);
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step two: Couldn't init audio: %s", Mix_GetError());
    }
}

void Sound::playMusic(int index)
{
    if (mMusicBank.size() == 0)
        return;

    if (mMusicBank[index] != nullptr)
    {
        Mix_PlayMusic(mMusicBank[index], -1);
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step two: Couldn't init audio: %s", Mix_GetError());
    }
}

void Sound::Play_Pause()
{
    if (!mPaused)
    {
        Mix_PauseMusic();
        mPaused = true;
    }
    else if (mPaused)
    {
        Mix_ResumeMusic();
        mPaused = false;
    }
    else
    {
        return;
    }
}

Sound& Sound::getInstance()
{
    static Sound instance;
    return instance;
}
