//
//  aVideo.h
//
//  Created by psm on 2/4/14.


#pragma once
#include "ofMain.h"
#include "aHotspot.h"
#include "DataStructs.h"

class aVideo {
    
public:
    aVideo(sVidData _movData);
    
    void draw(ofPoint _nodeLoc);
    void update();
    void start();
    void finish();
    
    //asked for by Node, for possible Hotspot
    ofVec4f dimensions;
    bool    centered;
    
private:
    sVidData vidData;
    ofVideoPlayer mov;

    int W, H; //size of the media
    ofPoint locXY;
    
    string assetFile;
    
    void updateFade();
    float alphaPct, currentAlpha;
    int direction;
    float inrate,outrate;
    
    ofColor baseClr, highlightClr, selectedClr, currentColor;

    aHotspot* hotSpt = NULL;
    //only activated if the Node containing this asset includes a link

};