//  entirely new asset class
//  aTextCntnr.cpp   REV pm091418
//  scenesREV
//
//  Created by Victoria Yi on 9/11/14.

#include "aTextCntnr.h"
#include "ofBitmapFont.h"

extern int winX; //defined in Scene
extern int winY;

aTextCntnr::aTextCntnr(sTextContainerData _data, int _scenenum, int _nodenum){
    nodeNum = _nodenum;
    scenenum = _scenenum;
    
    displayedName=_data.DISPNAME;
    
    locXY.set(ofPoint(_data.X,_data.Y));
    
    W = ofBitmapStringGetBoundingBox(displayedName,locXY.x,locXY.y).width;
    H = ofBitmapStringGetBoundingBox(displayedName,locXY.x,locXY.y).height;
    
    dimensions[0] = _data.X;
    dimensions[1] = _data.Y;

    dimensions[2] = W;
    dimensions[3] = H;
    
    centered = (_data.ALIGN=="C");
    visible = _data.VISIBLE;
    rawcontentstring =_data.CONTENTS; //a "|"-delimited list of what's inside
    // must happen last parseContents();
    
    clrLightOff_base = ofColor(_data.R2,_data.G2,_data.B2);
    clrLightOff_hover = ofColor(_data.R2,_data.G2,_data.B2,190);
    clrLightOn_base = ofColor(_data.R,_data.G,_data.B);
    clrLightOn_hover = ofColor(_data.R,_data.G,_data.B,200);
    
    opened=false;
    lightOn=true;
    mouseIsOver=false;
    
    parseContents(); //1002 and an earlier version
    
    printf("TextCntnr instance complete for %s at %0.f %0.f\n",displayedName.c_str(), locXY.x, locXY.y);
}

void aTextCntnr::registerHotspot(aHotspot* _myhtspt) { //from Node during creation
    if (_myhtspt!=NULL) {
        myHotspot=_myhtspt;
        myHotspot->setVisible(true); //also should be disabling phantom clicks
        printf("Text regsterHS  for %s ",displayedName.c_str());
        cout<<"ptr="<<myHotspot<<endl;
    } else {
        printf("OOPS, you tried to register a NULL hotspot pointer in TextContainer asset %s\n",displayedName.c_str());
    }
}

void aTextCntnr::parseContents() {
    //look thru rawcontentstring for the | and break into a vector
    contents=ofSplitString(rawcontentstring, "|");
    
    int x = locXY.x+INDENT; //v.Yi int x = winX+locXY.x+W+10;
    
    for (int i=0; i<contents.size(); i++) {
        
        int y = locXY.y+LINESP+(i*LINESP); //v.Yi int y = winY+locXY.y+(i*LINESP);
        
        int W = ofBitmapStringGetBoundingBox(contents[i],x,y).width;
        int H = ofBitmapStringGetBoundingBox(contents[i],x,y).height;
        
        ofVec4f dimensions;
        dimensions[0] = x;
        dimensions[1] = y;
        
        dimensions[2] = W;
        dimensions[3] = H;
        
        int itemNum=(nodeNum*10)+i; //max 9 items //last digit is "idnum" of this item
        
        //ofVec4f _dims, bool _centered, int _id, int _nodenum, int _scenenum
        itemHotspots.push_back(new aHotspot(dimensions, false, i, nodeNum, scenenum));
        textItems.push_back(new aText(dimensions, contents[i], clrLightOff_base, clrLightOff_hover, clrLightOn_base, clrLightOn_hover, itemNum ));
        textItems[i]->registerHotspot(itemHotspots[i]); //092614
    }

    //each item is represented by a aText asset, and entire collection is listed in a vector for easy scanning
    printf("TextCntnr %s gets a contents list of size %lu\n",displayedName.c_str(), contents.size());
}

void aTextCntnr::setVisible(){ //might set from node?
    visible=!visible;
    if (myHotspot!=NULL) myHotspot->setVisible(visible); //092914
}
void aTextCntnr::setLight() { //092514
    lightOn = !lightOn;
    //also tell your contents... //pm 0930
    for (int i=0; i<textItems.size(); i++) {
        textItems[i]->setLight();
    }
}
void aTextCntnr::setOpened(){ //called by Node
    opened=!opened;
}

void aTextCntnr::update() { //via Asset from testApp update() loop
    if (visible) {
        if (myHotspot!=NULL) mouseIsOver=myHotspot->update(); //092614 //092914
        
        if (mouseIsOver==false) {
            if(lightOn) {clr=clrLightOn_base;}
            else {clr=clrLightOff_base;}
        } else {
            if(lightOn) {clr=clrLightOn_hover;}
            else {clr=clrLightOff_hover;}
        }
        
        if (opened) {
            //also update all your "children"
            for (int i=0; i<textItems.size(); i++) {
                textItems[i]->update();
            }
        }
    }
}

void aTextCntnr::draw(ofPoint _nodeLoc) {
    //printf("aTextCntnr %s drawing at %0.f %0.f\n",displayedName.c_str(), locXY.x, locXY.y);

    if (visible) {
        ofSetColor(clr);
        ofDrawBitmapString(displayedName,winX+_nodeLoc.x, winY+_nodeLoc.y);
        
        if (opened) {
            for (int i=0; i<textItems.size(); i++) {
                textItems[i]->draw();
            }
        }
    }
}

//1003 many places not noted, getting taken items and loading onto a list in Core

int aTextCntnr::getNumItems() {
    return textItems.size();
}

aText* aTextCntnr::getTakenItem(int _i) {
    if (textItems[_i]->taken) {
        return textItems[_i];
    } else {
        return NULL;
    }
}
