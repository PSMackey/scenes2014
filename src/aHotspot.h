//
//  aHotspot.h
//
//  Created by psm on 2/4/14.


#pragma once
#include "ofMain.h"
#include "Events.h" 
#include "DataStructs.h"

class aText;

class aHotspot {
    
public:
    aHotspot();
    
    aHotspot(sLinkData _LD, int _nodenum, int _scenenum, string _scname);
    aHotspot(int _nodenum, int _scenenum);
    aHotspot(ofVec4f _dims, bool _centered, int _id, int _nodenum, int _scenenum);
    
    bool update();
    void mousePressed(Events &e);
    void mouseReleased(Events &e);
//    bool checkMouseOver();
    
    void setDims(ofVec4f _dims, bool _centered);
    void setDims(int _w, int _h);
    
    void setAudioPlayOnClick(string _file, bool _loop);
    
    ofPoint getLoc();
    ofRectangle perimeter;
    
    void setVisible(bool _torf);
    
    void registerAsset(aText* _assetptr); //092614
    aText* myAssetPtr; //092614
    
    
private:
    bool visible; //091814
    bool wasEntered;
    
    int W, H; //size of the media
    ofPoint locXY;
    bool    centered;
    //moved fer testing  ofRectangle perimeter;
    
    bool draggable;
    
    string myAssetsName;
    string mySceneName;
    int myNodeNum, mySceneNum, idnum;
    
    e_linkmodes  myLinkType;
    string myLinkInfoStr;  //091814 changed names
    void changeScene();

    
    ofColor baseClr, highlightClr, selectedClr, currentColor;
    
    ofSoundPlayer* audio = NULL;
    bool looping;
    
    
    bool mouseIsOver, mouseIsDown, isDragged;
    ofPoint mseDnOffset;
    string link;

    //int winX, winY, winW, winH;
};