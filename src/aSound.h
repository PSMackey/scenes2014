//
//  aSound.h
//
//  Created by psm on 2/4/14.


#pragma once
#include "ofMain.h"
#include "aHotspot.h"
#include "DataStructs.h"

class aSound {
    
public:
    aSound(sAudioData _sndData);
    
    //void draw();
    void update();
    void start();
    void finish();
    
private:
    void updateFade();
    int direction;
    float currentVolume;
    
    sAudioData theAudioData;
    ofSoundPlayer snd;

    string assetFile;
    bool loop;      //default F
    bool playing;   //default T
    float maxvolume;   //default 1.0
    float pan;      //default 0.0
    float outrate;  //fade-out period in seconds
    float inrate;   //fade-in " " "
    
    
        
};