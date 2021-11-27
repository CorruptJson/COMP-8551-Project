#pragma once
#include <vector>
#include <SDL_mixer.h>
class Sound 
{
public:
    Sound();
    ~Sound();

    void addSound(const char* path);

    void addMusic(const char* path);

    void playSound(const int soundIndex);

    //void playSound(const char* path);

    //void playMusic(const char* path);

    void playMusic(const int musicIndex);

    void Play_Pause();

private:
    std::vector<Mix_Chunk*> mSoundBank;
    std::vector<Mix_Music*> mMusicBank;

    bool mPaused;
    bool mPlaying;
};
