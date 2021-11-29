#pragma once
#include <vector>
#include <string>
#include <SDL_mixer.h>
#include "Types.h"
// enum for musics
enum soundEffects {
    SHOOT,
    JUMP,
    ENEMYDEATH,
    PLAYERDEATH,
    FLAMEDEATH
};
class Sound 
{
public:
    ~Sound();
    void loadSfx(std::vector<std::string> sfxPaths);
    void loadMusic(std::vector<std::string> musicPaths);
    
    //void playSound();

    void playSound(int index);
    void playSound(const char* path);

    void playMusic(int index);
    void playMusic(const char* path);

    void Play_Pause();
    static Sound& getInstance();
private:
    Sound();
    std::vector<Mix_Chunk*> mSFXBank;
    std::vector<Mix_Music*> mMusicBank;

    bool mPaused = false;
    //bool mPlaying;
};
