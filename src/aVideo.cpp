//
//  aVideo.cpp
//
//  Created by psm on 2/4/14.


#include "aVideo.h"

aVideo::aVideo(sVidData _data){
    vidData=_data;
    
    assetFile=_data.FILE;
    
    mov.loadMovie(assetFile);
    //mov.play(); //temp, use (_data.PLAYING)
    //playing before you need it might eat some system juice
    
    if (_data.LOOP) { mov.setLoopState(OF_LOOP_NORMAL); }
    else { mov.setLoopState(OF_LOOP_NONE); } //how about PALINDROME!?
    
    mov.setVolume(0.0); //
    W=mov.width;
    H=mov.height;
    
    //TODO: inrate & outrate
    outrate=_data.OUTRATE;    //fade-out speed 0.1 to 1.0, default 0.0
    inrate= _data.INRATE;    //fade-in " " ", default 0.0
    
    locXY.set(ofPoint(_data.X,_data.Y));
    centered = false;
    //TODO: should get property value from struct
    
    dimensions[0] = _data.X;
    dimensions[1] = _data.Y;
    dimensions[2] = W;
    dimensions[3] = H;
    
    
    currentAlpha = 255;
    baseClr = ofColor(255,currentAlpha);
    highlightClr = ofColor(255,0,255);
    
    printf("Video instance made for image %s\n",assetFile.c_str());
}

//called from Node
void aVideo::start() {
    //need to re-set this each time? nope
    //TODO video is not looping
    if (vidData.LOOP) { mov.setLoopState(OF_LOOP_NORMAL); }
    else { mov.setLoopState(OF_LOOP_NONE); }

    if (!(inrate>0)) { //if no fadein rate set, just set alpha to max
        mov.play();
        alphaPct = 1.0;
        currentAlpha=255;
        baseClr = ofColor(255,currentAlpha);
        //TODO: fade volume too if not silent
        direction=0; //dont bother to adjust fade level
    } else {
        //doFadeIn
        mov.play();
        alphaPct = 0.0;
        currentAlpha=0;
        baseClr = ofColor(255,currentAlpha);
        direction=1;
    }
}

void aVideo::finish() { //doFadeOut
    if (!(inrate>0)) { //just stop (but stay visible)
        mov.stop();
        currentAlpha=255;
        direction=0; //dont bother
    } else {
        //fade out
        direction=-1;
    }
    //TODO: wip-debug fading after you figure out how
    //      to pause a scene change until all fades have completed haha yikes!
}


void aVideo::update() {
    mov.update();
    updateFade();
}

void aVideo::draw(ofPoint _nodeLoc) {
    //printf("Video instance drawing %s at %0.f %0.f\n",assetFile.c_str(), _nodeLoc.x, _nodeLoc.y);
    
    ofSetColor(baseClr);
    mov.draw(_nodeLoc);
}

void aVideo::updateFade() {
    //keep checking the clip's volume
    switch (direction) {
        case 1 :
            //should be fading-in, so add to the density
            alphaPct += inrate;
            if (alphaPct >= 1.0) {
                direction = 0;//stop fading
                alphaPct = 1.0;
            }
            currentAlpha=255.0*alphaPct;
            baseClr = ofColor(255,currentAlpha);
            break;
        case -1 :
            //should be fading out, so reduce the opacity
            alphaPct -= outrate;
            if (alphaPct <= 0.0) {
                direction = 0;//stop fading
                alphaPct = 0.0;
                mov.stop();
            }
            currentAlpha=255.0*alphaPct;
            baseClr = ofColor(255,currentAlpha);
            break;
    }
}

