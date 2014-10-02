//
//  aPicture.cpp
//  A possible asset of a Node
//  Created by psm on 2/4/14.


#include "aPicture.h"

aPicture::aPicture(sPictData _data){

    assetFile=_data.FILE;
    //also, deal with missing file?
    
    pict.loadImage(assetFile);
    W=pict.width;
    H=pict.height;
    
    locXY.set(ofPoint(_data.X,_data.Y));
    
    dimensions[0] = _data.X;
    dimensions[1] = _data.Y;
    dimensions[2] = W;
    dimensions[3] = H;
    
    centered = (_data.ALIGN=="C");
    draggable = (_data.DRAG=="T");
    visible=true;       //092614
    mouseIsOver=false;
    
    baseClr = ofColor(255);
    highlightClr = ofColor(255,0,255);
    
    printf("Picture instance made for image %s at %0.f %0.f\n",assetFile.c_str(), locXY.x, locXY.y);
}

void aPicture::registerHotspot(aHotspot* _myhtspt) { //from Node during creation
    if (_myhtspt!=NULL) {
        myHotspot=_myhtspt;
        myHotspot->setVisible(true); //this also should be disabling phantom clicks
        printf("PPPPicture %s gets a Hotspot\n",assetFile.c_str());
    } else {
        printf("OOPS, you tried to register a NULL hotspot pointer in Picture asset %s\n",assetFile.c_str());
    }
}
void aPicture::setVisible(){ // set from node via scene click 091814
    visible=!visible;
    if (myHotspot!=NULL) myHotspot->setVisible(visible); //092914
}

void aPicture::update() { //via Node from Core update() loop
    if (visible) {
       if (myHotspot!=NULL)  mouseIsOver=myHotspot->update(); //092914
    }
}

void aPicture::draw(ofPoint _nodeLoc) {
    //printf("Picture %s drawing at %0.f %0.f\n",assetFile.c_str(), locXY.x, locXY.y);
    if (visible) {
        ofSetColor(baseClr);
        if (centered) pict.setAnchorPercent(0.5, 0.5); //works?
        //pict.draw(locXY);
        pict.draw(_nodeLoc);
    }
}
