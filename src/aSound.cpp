//
//  aSound.cpp
//
//  Created by psm April 2014.


#include "aSound.h"

aSound::aSound(sAudioData _data){

    //property values read/set in XML, saved in the sAudioData struct
    assetFile=  _data.FILE;
    loop=       _data.LOOP;       //default F
    playing=    _data.PLAYING;   //default T
    //you have start() which is called always, but may need to keep asset paused/silent/invisible until some other event
    
    maxvolume=  _data.MAXVOL;   //default 1.0
    pan=    _data.PAN;         //default 0.0
    outrate=_data.OUTRATE;    //fade-out speed 0.1 to 1.0, default 0.0
    inrate= _data.INRATE;    //fade-in " " ", default 0.0
    direction = 0;
    
    snd.setLoop(loop);
    snd.loadSound(assetFile);
    
//    if ( (playing)&&(! inrate>0) ) {
//        snd.play();
//        //WIP ok so long as this object is only born at moment when hearing sound is appropriate, which it is not in this version
//        snd.setVolume(maxvolume);
//    }
    
    printf("aSound instance made for %s\n",assetFile.c_str());
}
//TODO: deal with possibility of "Playing=false"

//called from Node
//start() is called always, but may need to keep asset paused/silent/invisible until some other event, so use PLAYING flag
void aSound::start() { //doFadeIn
    if (!inrate>0) { //if no fadein rate set, just set vol to 1
        snd.play();
        snd.setVolume(maxvolume);
        currentVolume=maxvolume; //good housekeeping
        direction=0; //dont bother to adjust fade level
    } else {
        //start fading in
        snd.play();
        snd.setVolume(0);
        currentVolume=0;
        direction=1;
    }
}

void aSound::finish() { //doFadeOut
    if (!outrate>0) { //if not outrate, just shut up
        snd.stop();
        snd.setVolume(0);
        direction=0; //dont bother to "update"
        currentVolume=0; //good housekeeping
    } else {
        //currentVolume=1; //let fade from current level
        direction=-1;
    }
    
    //TODO: wip-debug fading
    
}

void aSound::update() {
    updateFade();
}
void aSound::updateFade() {
    //keep checking the clip's volume
    switch (direction) {
        case 1 :
            //should be fading-in, so add to the volume
            currentVolume += inrate;
            if (currentVolume >= maxvolume) {
                direction = 0;//stop fading
                currentVolume = maxvolume;
            }
            //printf("aAudio for %s fading in %.2f\n",assetFile.c_str(),currentVolume);
            snd.setVolume(currentVolume);
            break;
        case -1 :
            //should be fading out, so reduce the opacity
            currentVolume -= outrate;
            if (currentVolume <= 0.0) {
                direction = 0;//stop fading
                currentVolume = 0.0;
            }
            //printf("aAudio for %s fading out %.2f\n",assetFile.c_str(),currentVolume);
            snd.setVolume(currentVolume);
            break;
    }
}


