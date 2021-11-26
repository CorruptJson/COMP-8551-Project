#include "Sound.h"
#include <SDL.h>
#include <iostream>
//#include <iostream>
//#include <windows.h>
//#include <mmsystem.h>

//void Sound::playSoundOnce() {
//    //
//}
//
//void Sound::loopSound() {
//    bool played = PlaySound(TEXT("brionac.wav"), NULL, SND_FILENAME | SND_ASYNC);
//}

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

void Sound::addSound(const char* path)
{
    Mix_Chunk* tmpChunk = Mix_LoadWAV(path);
    if (tmpChunk != nullptr)
    {
        mSoundBank.push_back(tmpChunk);
        std::cout << (mSoundBank.size() - 1) << "Sound is ready, path: " << path << '\n';
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Step two: Couldn't init audio: %s", Mix_GetError());
    }
}

void Sound::playSound()
{
    Mix_PlayChannel(-1, mSoundBank[0], 0);
    std::cout << "Played Sound " << '\n';
}
