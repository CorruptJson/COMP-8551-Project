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
    SDL_Quit();
}

//void Sound::addSound(const char* path)
//{
//    Mix_Chunk* tmpChunk = Mix_LoadWAV(path);
//    if (tmpChunk != nullptr)
//    {
//        mSoundBank.push_back(tmpChunk);
//    }
//    else
//    {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step two: Couldn't init audio: %s", Mix_GetError());
//    }
//}
//
//void Sound::playSound()
//{
//    Mix_PlayChannel(-1, mSoundBank[1], 0);
//}

void Sound::playSound(const char* path)
{
    Mix_Chunk* tmpChunk = Mix_LoadWAV(path);
    if (tmpChunk != nullptr)
    {
        Mix_PlayChannel(-1, tmpChunk, 0);
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step two: Couldn't init audio: %s", Mix_GetError());
    }
}

void Sound::playMusic(const char* path)
{
    Mix_Music* tmp_music = Mix_LoadMUS(path);

    if (tmp_music != nullptr)
    {
        Mix_PlayMusic(tmp_music, -1);
        mPlaying = true;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step two: Couldn't init audio: %s", Mix_GetError());
    }
}

void Sound::Play_Pause()
{
    if (mPlaying && !mPaused)
    {
        Mix_PauseMusic();
        mPaused = true;
    }
    else if (mPlaying && mPaused)
    {
        Mix_ResumeMusic();
        mPaused = false;
    }
    else
    {
        return;
    }
}
