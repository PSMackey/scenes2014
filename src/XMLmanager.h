//
//  XMLmanager.h
//  xml3scenesExample
//
//  Created by Peter Mackey on 14/02/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "DataStructs.h"

class XMLmanager {
    
public:
    XMLmanager();
    
    bool loadXMLfile(string _file);
    
    int getNumOfScenes();
    string getBG(int _scenenum, int _nodei);
    string getSceneName(int _scenenum);
    
    int getNumNodes(int _scenenum);
    string getNodeName(int _scenenum, int _nodenum);
    int getAssetCountInNode(int _scenenum, int _nodenum);
    
    //bool hasAreaTag(int _scenenum, int _i);
    
    bool nodeHasLink(int _scenenum, int _ni);
    bool nodeHasThisTag(e_types _tag, int _scenenum, int _ni);
    
    int getNumAnimations(int _scenenum, int _nodei);
   // bool hasAnimationTag(int _scenenum, int _i);
    sAnimData getAnimation(int _scenenum, int _nodei);
    sAnimData animdata;

    ofVec4f getArea(int _scenenum, int _nodei);
    
    string getCustom(int _scenenum, int _nodei);
    
    sPictData getPict(int _scenenum, int _nodei);
    sPictData pictdata;
    
    sTextData getText(int _scenenum, int _nodei);
    sTextData textdata;
    
    sTextContainerData getTextContainer(int _scenenum, int _nodei);
    sTextContainerData textcntnrdata;
    
    sLinkData getLink(int _sceneNum, int _nodei);
    
    sAudioData getAudio(int _sceneNum, int _nodei);
    string getAudioMode(int _scenenum, int _ni);
    
    sVidData getVideo(int _sceneNum, int _nodei);
    sVidData viddata;
    
    int getNumImages(int _scenenum);
    //sPictData getNonbranchImage(int _scenenum, int _i);
    
    int getNumTags(string _TAG, int _scenenum);
    
private:
    
    ofxXmlSettings XML;
    
    string xmlStructure;
    
    bool firstPass;
    
    int numSceneTags;
    
};