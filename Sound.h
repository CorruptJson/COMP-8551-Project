//#include <iostream>
//#include <windows.h>
//using namespace std;
//
//class Sound {
//public:
//
//    Sound(string inpSound) {
//        soundUsing = inpSound;
//    }
//
//    void playSoundOnce();
//    void loopSound();
//
//private:
//    string soundUsing;
//};

#pragma once
#include <vector>
#include <SDL_mixer.h>
class Sound 
{
public:
    Sound();
    ~Sound();

    void addSound(const char* path);

    void playSound();

private:
    std::vector<Mix_Chunk*> mSoundBank;
};
