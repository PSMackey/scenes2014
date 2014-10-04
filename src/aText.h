//
//  aText.h
//
//  Created by Victoria Yi on 9/11/14.


#pragma once
#include "ofMain.h"
#include "aHotspot.h"
#include "DataStructs.h"


class aText {
    
public:
    aText(sTextData _data, int _nodenum);
    aText(ofVec4f _dimensions, string _content, ofColor _clrLightOff_base, ofColor _clrLightOff_hover, ofColor _clrLightOn_base, ofColor _clrLightOn_hover,int _itemnum);
    
    void draw(ofPoint _nodeLoc);
    void draw();
    void update();
    void setVisible();  //091814
    void setLight(); //092514
    
    //asked for by Node, for possible Hotspot
    ofVec4f dimensions;
    bool    centered;
    bool    draggable;
    
    void registerHotspot(aHotspot* _myhtspt); //092614
    void setTaken(); //092614
    bool taken;
    string getName();
    
private:
    int nodeNum;            //092614 //1002
    aHotspot* myHotspot=NULL;
    
    sTextData theTextData;
    string text;
    bool visible;  //091814
    bool lightOn; //092514
    bool mouseIsOver;

    int W, H, Y; //size of the media
    ofPoint locXY;
    
    string assetFile;
    
    ofColor clr;
    ofColor clrLightOn_base, clrLightOn_hover, clrLightOff_base, clrLightOff_hover;
    
};