//
//  aTextCntnr.h   REV pm091418
//
//  Created by Victoria Yi on 9/11/14.


#pragma once
#include "ofMain.h"
#include "aHotspot.h"
#include "DataStructs.h"
#include "aText.h"


class aTextCntnr {
    
public:
    aTextCntnr(sTextContainerData _data, int _scenenum, int _nodenum);
    
    void draw(ofPoint _nodeLoc);
    void update();
    
    //asked for by Node, for possible Hotspot
    ofVec4f dimensions;
    bool    centered;
    
    void setOpened();
    void setVisible();
    void setLight();
    void registerHotspot(aHotspot* _myhtspt);
    
    aText* getTakenItem(int _i); //1003 both
    int getNumItems();
    
private:
    aHotspot* myHotspot=NULL; //092914
    int nodeNum, scenenum; //pm1001 cap N
    const int LINESP=18;  //line spacing for list of contents
    const int INDENT=5;
    
    bool visible;
    bool lightOn;
    bool opened;
    bool mouseIsOver;
  
    string displayedName;
    
    int W, H; //size of the media
    ofPoint locXY;
    
    vector<string> contents;
    void parseContents();
    vector<aHotspot*> itemHotspots; //092614
    vector<aText*> textItems;       //092614
    
    string rawcontentstring;
    //string assetName;
    
    ofColor clr;
    ofColor clrLightOn_base, clrLightOn_hover, clrLightOff_base, clrLightOff_hover;
    
};