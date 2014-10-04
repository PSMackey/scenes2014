//
//  aText.cpp
//  xmlReadScene11
//
//  Created by Victoria Yi on 9/11/14.
//
//  visible REV pm //091814


#include "aText.h"
#include "ofBitmapFont.h"

extern int winX; //defined in Scene
extern int winY;

aText::aText(sTextData _data, int _nodenum){
    nodeNum=_nodenum;
    assetFile=_data.FILE;
    //TODO: call this displayedName
    text = assetFile;
    taken = false;
    
    locXY.set(ofPoint(_data.X,_data.Y));
    
    W = ofBitmapStringGetBoundingBox(text,locXY.x,locXY.y).width;
    H = ofBitmapStringGetBoundingBox(text,locXY.x,locXY.y).height;
    
    dimensions[0] = _data.X;
    dimensions[1] = _data.Y;
    dimensions[2] = W;
    dimensions[3] = H;
    
    centered = (_data.ALIGN=="C");
    draggable = (_data.DRAG=="T");
    visible = _data.VISIBLE;
    mouseIsOver = false;
    
    clrLightOff_base = ofColor(_data.R2,_data.G2,_data.B2);
    clrLightOff_hover = ofColor(_data.R2,_data.G2,_data.B2,200);
    clrLightOn_base = ofColor(_data.R,_data.G,_data.B);
    clrLightOn_hover = ofColor(_data.R,_data.G,_data.B,200);

    lightOn=true;
    
    printf("Text instance made for text %s at %0.f %0.f\n",assetFile.c_str(), locXY.x, locXY.y);
}

aText::aText(ofVec4f _dims, string _content, ofColor _clrLightOff_base, ofColor _clrLightOff_hover, ofColor _clrLightOn_base, ofColor _clrLightOn_hover,int _itemnum) {
    dimensions=_dims; //may be asked for by Node for Hotspot
    nodeNum=_itemnum;
    text=_content;
    taken=false;
    locXY.set(ofPoint(dimensions[0],dimensions[1]));
    W = dimensions[2];
    H = dimensions[3];
    centered = false;
    draggable = false;
    visible = true; //091814 could add to XML properties
    mouseIsOver=false;
    clrLightOff_base = _clrLightOff_base;
    clrLightOff_hover = _clrLightOff_hover;
    clrLightOn_base = _clrLightOn_base;
    clrLightOn_hover = _clrLightOn_hover;
    
    lightOn=true;
    
    printf("TextItem made for text %s as item#%i at %0.f %0.f\n",text.c_str(), nodeNum, locXY.x, locXY.y);
}

void aText::registerHotspot(aHotspot* _myhtspt) { //from Node during creation
    if (_myhtspt!=NULL) {
        myHotspot=_myhtspt;
        myHotspot->setVisible(true); //this also should be disabling phantom clicks
        myHotspot->registerAsset(this);
        printf("Text registerHS  for %s ",text.c_str());
        cout<<"ptr="<<myHotspot<<endl;
    } else {
        printf("OOPS, you tried to register a NULL hotspot pointer in Text asset %s\n",text.c_str());
    }
}

string aText::getName(){ //called by your Hotspot 092614
    return text;
}

void aText::setTaken() { //092614 msg from Hotspot
    taken=true;  //<--------could poll for this property from Core
    visible=false;
    if (myHotspot!=NULL) myHotspot->setVisible(false); //092914
    printf("HARK! aText asset %s has been TAKEN\n",text.c_str());
}

void aText::setVisible(){ //might set from node 091814
    visible=!visible;
    if (myHotspot!=NULL) myHotspot->setVisible(visible); //092914
}

void aText::setLight() { //092514
    lightOn = !lightOn;
}

void aText::update() { //via Asset from testApp update() loop
    mouseIsOver=false;
    if (visible) {
       if (myHotspot!=NULL) mouseIsOver=myHotspot->update(); //092914
        
        if (mouseIsOver==false) {
            if(lightOn) {clr=clrLightOn_base;}
            else {clr=clrLightOff_base;}
        } else {
            if(lightOn) {clr=clrLightOn_hover;}
            else {clr=clrLightOff_hover;}
        }
    }
    //1002 return mouseIsOver;  //needed if this is a text Item in a textcontainer
}

void aText::draw() {  //092614 version for textitems in a container
    //printf("Picture %s drawing at %0.f %0.f\n",assetFile.c_str(), locXY.x, locXY.y);
    ofSetColor(clr);
    
    if(visible) {
        ofDrawBitmapString(text,locXY+ofPoint(winX,winY));
    }
}

void aText::draw(ofPoint _nodeLoc) {
    //printf("Picture %s drawing at %0.f %0.f\n",assetFile.c_str(), locXY.x, locXY.y);
    ofSetColor(clr);
    if(visible) {
        ofDrawBitmapString(text,winX+_nodeLoc.x, winY+_nodeLoc.y);
    }
    
}



