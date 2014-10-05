  //  Scene.cpp            //
 //  scenes1002 - psm //  
///////////////////////////

#include "Scene.h"

int winX;
int winY;

Scene::Scene(XMLmanager* _XMLmgr, ofTrueTypeFont* _font12, int _i){
    sceneNum = _i; //index of this scene in the scenes array
    XMLmgr = _XMLmgr;
    font12 = _font12;
    
    sceneName = XMLmgr->getSceneName(sceneNum);
    printf("Making a Scene named %s\n",sceneName.c_str());
    
    ofBackground(0); //default black bg
    lightoff = false;
    
    //this event will be sent from any Hotspot with a REVEAL mode link
    ofAddListener(Events::revealassetevent, this, &Scene::revealAsset);
    ofAddListener(Events::lightswitchevent, this, &Scene::changeClrs); //092514
    
    //interface outlines
    winW = 1200;
    winH = 660;
    winX = (ofGetWidth()/2)-(winW/2);
    winY = (ofGetHeight()/2)-((winH+100)/2);
    dlgH = 100;
    window.set(winX,winY,winW,winH);
    dialogue.set(winX,winY+winH,winW,dlgH);
    
    //create Nodes for this scene==========================
    
    int nNodes = XMLmgr->getNumNodes(sceneNum);
    if ( nNodes>0 ) { //otherwise, what?
        for (int ni=0;ni<nNodes;ni++) {
            
            //notice: sceneNum is the xmlindex of this scene
            nodes.push_back(new Node(sceneNum, ni, XMLmgr)); //and font12
            //XMLmgr is a pointer
            
            //while looking, extract the BG data when you find it...
            if (nodes[ni]->isTheBG() ) {
                bgstring = nodes[ni]->getBGinfo();
                //if bgfile str is numeric, you want a solid color
                //otherwise it's a file, thus...
                if (bgstring!="") {
                    //if it contains a "." it is a file name
                    if (ofIsStringInString(bgstring,".")) { //error check??
                        bg.loadImage(bgstring);
                        hasBGfile=true;
                    } else if (ofIsStringInString(bgstring,",")) {
                        //TODO: consider not using , as delim since this MAY confuse CSV-style translation
                        bgRGB=ofSplitString(bgstring, ",");
                        bgR = ofToInt(bgRGB[0]);
                        bgG = ofToInt(bgRGB[1]);
                        bgB = ofToInt(bgRGB[2]);
                        BGcolor=ofColor(bgR,bgG,bgB);
                    } else {
                        //check for legit value?
                        BGcolor=ofColor(ofToInt(bgstring));
                    }
                }
            } //end if you found the background
            
        }
    }
}

void Scene::arrive(){
    printf("\nArriving at Scene#%i, %s_________________\n",sceneNum,sceneName.c_str());
    printf("This scene has %lu nodes.\n",nodes.size());
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->arrive();
    }
}
void Scene::depart(){
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->depart();
    }
    printf("____________________Departing from Scene#%i\n",sceneNum);
}

string Scene::getName() {
    return sceneName;
}

void Scene::update(ofPoint _mse) {
        for (int i=0; i<nodes.size(); i++) {
            nodes[i]->update( _mse );
        }
}

void Scene::draw() {
    
    if (hasBGfile) { bg.draw(0,0); }
    else { ofBackground(BGcolor); }
    
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->draw();
    }
    
    //interface borders
    ofNoFill();
    ofSetColor(0);
    ofRect(window);
    ofRect(dialogue);
}

//1003
vector<aText*> Scene::lookForTakenItems() { //called by Core
    //nodes with Text assets will return individual items
    vector<aText*> myNodesTakenItems;
    for (int i=0; i<nodes.size(); i++) {
        aText* tiPtr = NULL;
        tiPtr = nodes[i]->getTakenItem();
        if (tiPtr != NULL) {
            myNodesTakenItems.push_back(tiPtr);
        }
    }
    return myNodesTakenItems;
}

//1003
vector<aText*> Scene::lookForTakenContainerItems() {  //also called by Core
    vector<aText*> myNodesTakenContainerItems;
    
    //nodes with TextContainers will return vectors of possibly multiple items
    
    vector<aText*> itemsFromNodeContainer;
    //go through each returned vector and add its items to myNodesTakenContainerItems
    for (int i=0; i<nodes.size(); i++) {
        itemsFromNodeContainer = nodes[i]->getTakenContainerItems(); //come back as vectors
        //insert that vector into myNodesTakenContainerItems
        vector<aText*>::iterator ittse = itemsFromNodeContainer.end();
        vector<aText*>::iterator ittsb = itemsFromNodeContainer.begin();
        
        myNodesTakenContainerItems.insert(myNodesTakenContainerItems.end(), ittsb, ittse);
        itemsFromNodeContainer.clear(); //empty it for next pass
    }
    return myNodesTakenContainerItems;
}

//091814
void Scene::revealAsset(Events &e) {
    // event info in this package
    //e.targetscenenum;
    //e.targetnodename;
    if (e.targetscenenum==sceneNum) {
        //tell appropriate node in this scene to show/hide itself
        for (int i=0; i<nodes.size(); i++) {
            string n = nodes[i]->getName();
            if (n == e.targetnodename) {
                nodes[i]->setVisible();
            }
        }
    }
}

void Scene::changeClrs(Events &e) {
    if (e.targetscenenum==sceneNum) { //pm 0930 do this first to "qualify" this scene as the intended target
        
        for (int i=0; i<nodes.size(); i++) {
            nodes[i]->setLight();
        }
        
        lightoff = !lightoff;
        if(lightoff) {
            string bgstring2 = e.lightcolor;
            bgRGB2=ofSplitString(bgstring2, ",");
            bgR2 = ofToInt(bgRGB2[0]);
            bgG2 = ofToInt(bgRGB2[1]);
            bgB2 = ofToInt(bgRGB2[2]);
            BGcolor=ofColor(bgR2,bgG2,bgB2);
        } else {
            if (ofIsStringInString(bgstring,",")) {
                BGcolor=ofColor(bgR,bgG,bgB);
            } else {
                BGcolor=ofColor(ofToInt(bgstring));
            }
        }
    }
}
