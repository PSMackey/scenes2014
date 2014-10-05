//  Scene.h               //
//  scenes1002 - psm //
//////////////////////////


#pragma once


#include "ofMain.h"

#include "XMLmanager.h"
#include "DataStructs.h"
#include "Node.h"
#include "aText.h" //1003
#include "Events.h" //091814

class Scene {

public:
    Scene(XMLmanager* _XMLmgr, ofTrueTypeFont* _font12, int _i);
    
    void update(ofPoint _mse);
    void draw();
    
    string getName();
    string bgstring;
    
    void arrive();
    void depart();
    
    vector<aText*> lookForTakenItems(); //1003 both
    vector<aText*> lookForTakenContainerItems();
    
private:
    XMLmanager* XMLmgr;
    ofTrueTypeFont* font12;
    
    vector<Node*> nodes;
    
    string sceneName;
    int sceneNum;
    
    ofImage bg;
    bool hasBGfile =  false;
    ofColor BGcolor = ofColor(0);
    vector<string> bgRGB, bgRGB2;
    int bgR, bgG, bgB, bgR2, bgG2, bgB2;
    
    //091814
    void revealAsset(Events &e);
    //092514
    void changeClrs(Events &e);
    bool lightoff;
    
    ofRectangle window, dialogue;
    int winW, winH;
    int dlgY, dlgH;
    
};