#pragma once
#include <vector>
#include <SDL_mixer.h>
class Sound 
{
public:
    Sound();
    ~Sound();

    //void addSound(const char* path);

    //void playSound();

    void playSound(const char* path);

    void playMusic(const char* path);

    void Play_Pause();

private:
    std::vector<Mix_Chunk*> mSoundBank;

    bool mPaused;
    bool mPlaying;
};
