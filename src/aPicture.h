//
//  aPicture.h
//
//  Created by psm on 2/4/14.


#pragma once
#include "ofMain.h"
#include "aHotspot.h"
#include "DataStructs.h"


class aPicture {
    
public:
    aPicture(sPictData _data);
    
    void draw(ofPoint _nodeLoc);
    void update();
    
    //asked for by Node, for possible Hotspot
    ofVec4f dimensions;
    bool    centered;
    bool    draggable;
    void registerHotspot(aHotspot* _myhtspt); //092614
    void setVisible();
    
private:
    aHotspot* myHotspot=NULL; //092914
    
    sPictData thePictData;
    ofImage pict;

    bool visible;  //091814
    bool mouseIsOver; //092614
    
    int W, H; //size of the media
    ofPoint locXY;
    
    string assetFile;
    
    ofColor baseClr, highlightClr, selectedClr, currentColor;
    
    
    
};