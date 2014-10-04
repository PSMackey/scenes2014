
//  Node.h



#pragma once

#include "ofMain.h"
#include "XMLmanager.h"
#include "DataStructs.h"

#include "aPicture.h"
#include "aText.h"
#include "aTextCntnr.h"
#include "aVideo.h"
#include "aSound.h"
#include "aAnimPlayer.h"
#include "aHotspot.h"

#include "Custom.h"

class Node {
    
public:
    Node(); //just in case, every class should have this default empty constructor
    
    Node(int _snum, int _xmlnodenum, XMLmanager* _xmlmgr);
    
    void initPict(sPictData _data);
    
    bool isTheBG();
    string getBGinfo();
    
    void update(ofPoint _mse);
    void draw();
//091814
//    void checkMousePress(ofPoint _mse);
//    bool checkMouseUp(ofPoint mse);
   
    int  getSelected();
    string getName();   //091814
    void reset();
    
    void arrive();
    void depart();
    void setVisible();  //091814
    void setLight(); //092514

    aText* getTakenItem(); //1003
    vector<aText*> getTakenContainerItems(); //1003
    
private:
    XMLmanager* XMLmgr;
    
    int nodeNum;        //unique, NOT the XML index
    string nodeName;    //optional
    int sceneNum;       //scene you belong to (the XML index)
    
    //only activated if the Node containing this asset includes a link
    aHotspot* hotSpot = NULL;
    //bool overhotspot;
    
    void assignDimensionsToHotspot();

    ofPoint nodeLoc;
    
    //assuming only one media type per Node
    //e_types nodeType; //may not need this
    bool nodeIsBG;  //psm04132014

    string BGinfo="";
    string customclassname="";
    
    sLinkData linkInfo;
    
    sAudioData audioInfo;
    aSound* audio = NULL;
    bool soundIsPlaying;
    
    sPictData pictInfo;
    aPicture* picture = NULL;
    
    sTextData textInfo;
    aText* text = NULL;
    
    //pm091814
    sTextContainerData textContInfo;
    aTextCntnr* textcntnr = NULL;
    void openTxtCntnr(Events &e);
    
    sVidData vidInfo;
    aVideo* video = NULL;
    
    sAnimData animInfo;
    aAnimPlayer* animation = NULL;
    
    bool canDrag;
    
    Custom* custom=NULL;
    
};
