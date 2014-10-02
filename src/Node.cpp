// Node.cpp
// multiple copies owned by each Scene
// types: BG, PICT, VIDEO, ANIM, AREA, TEST, AUDIO, CUSTOM
//TODO: add AREA [see projects "alphaAreaDetect" and "polyAreaDetect"]


#include "Node.h"

Node::Node(){}; //default empty constructor

Node::Node(int _snum, int _xmlnodei, XMLmanager* _xmlmgr ) {
    XMLmgr = _xmlmgr;
    sceneNum = _snum; //scene that contains this Node
    
    nodeNum = (_snum*100)+_xmlnodei;
    //arbitrary scheme to uniquely identify every Node in the project (assumes no more than 99 Nodes per scene haha)
    nodeIsBG=false;
    canDrag=false;
    
    //look at XML for this node. does it have an "id"(name)?
    nodeName = XMLmgr->getNodeName(_snum,_xmlnodei);
    printf("Scene %i made a new NODE %s num=%i\n",_snum,nodeName.c_str(),nodeNum);
    
    //now this Node reads its XML data and creates its assets
    //NOTICE a Node manages only one asset type
    
    //int nAssets = XMLmgr->getAssetCountInNode(_snum,_xmlnodei);
    
    if (XMLmgr->nodeHasThisTag(ANIM, sceneNum, _xmlnodei)) {
        animInfo = XMLmgr->getAnimation(sceneNum,_xmlnodei);
        nodeLoc = ofPoint(animInfo.X,animInfo.Y);
        animation = new aAnimPlayer(animInfo);
        if (animInfo.DRAG=="T") {canDrag=true;}
        
    }
    else if (XMLmgr->nodeHasThisTag(VIDEO, sceneNum, _xmlnodei)) {
        vidInfo = XMLmgr->getVideo(sceneNum,_xmlnodei);
        nodeLoc = ofPoint(vidInfo.X,vidInfo.Y);
        video = new aVideo(vidInfo);
        canDrag=vidInfo.DRAG;
        
    }
    else if (XMLmgr->nodeHasThisTag(PICT, sceneNum, _xmlnodei)) {
        pictInfo = XMLmgr->getPict(sceneNum,_xmlnodei);
        nodeLoc = ofPoint(pictInfo.X,pictInfo.Y);
        picture = new aPicture(pictInfo);
        if (pictInfo.DRAG=="T") {canDrag=true;}
        
    }
    else if (XMLmgr->nodeHasThisTag(TEXT, sceneNum, _xmlnodei)) {
        textInfo = XMLmgr->getText(sceneNum,_xmlnodei);
        nodeLoc = ofPoint(textInfo.X,textInfo.Y);
        text = new aText(textInfo,nodeNum);
        if (textInfo.DRAG=="T") {canDrag=true;}
        
    }
     //pm091814
    else if (XMLmgr->nodeHasThisTag(TEXTCONTAINER, sceneNum, _xmlnodei)) {
        textContInfo = XMLmgr->getTextContainer(sceneNum,_xmlnodei);
        nodeLoc = ofPoint(textContInfo.X,textContInfo.Y);
        textcntnr = new aTextCntnr(textContInfo,sceneNum,nodeNum);
        //be listening for messages from this
        //this event will be sent from any Hotspot
        ofAddListener(Events::opencontainerevent, this, &Node::openTxtCntnr);
    }
   else if (XMLmgr->nodeHasThisTag(BG, sceneNum, _xmlnodei)) {
        BGinfo = XMLmgr->getBG(sceneNum,_xmlnodei);
        nodeIsBG=true;
    }
    
    //does this Node have a LINK? make a Hotspot
    if (XMLmgr->nodeHasLink(sceneNum, _xmlnodei)) {
        linkInfo = XMLmgr->getLink(sceneNum,_xmlnodei);
        //does this look like valid data?
        if (linkInfo.INFO!="") {
            //activate a hotspot
            string sceneName = XMLmgr->getSceneName(sceneNum);
            hotSpot = new aHotspot(linkInfo, nodeNum, sceneNum, sceneName);
            
            //WIP if this Node has an AREA, feed that to the hotspot
            //otherwise give the hotspot the dims of the visual asset
            assignDimensionsToHotspot();
            
            if (picture!=NULL) { picture->registerHotspot(hotSpot); }
            else if (text!=NULL) { text->registerHotspot(hotSpot); }
            else if (video!=NULL) { video->update(); }
            else if (animation!=NULL) { animation->update(); }
            else if (textcntnr!=NULL) { textcntnr->registerHotspot(hotSpot); }
        }
    } else if (canDrag) {
        //hotspot has the logic to deal with mouseover & dragging
        hotSpot = new aHotspot(nodeNum, sceneNum);
        assignDimensionsToHotspot();
        if (picture!=NULL) { picture->registerHotspot(hotSpot); }
        else if (text!=NULL) { text->registerHotspot(hotSpot); }
    }
    
    if (XMLmgr->nodeHasThisTag(AUDIO, sceneNum, _xmlnodei)) {
        audioInfo = XMLmgr->getAudio(sceneNum,_xmlnodei);
        string mode = XMLmgr->getAudioMode(sceneNum,_xmlnodei);
        
        if (mode=="PLAYONCLICK") {
            hotSpot->setAudioPlayOnClick(audioInfo.FILE, audioInfo.LOOP);
            //the hotspot plays the sound
        } else {
            //the Node plays the sound (like ambiance)
            if (audio==NULL) {
                audio=new aSound(audioInfo);
            }
            //TODO: deal with ability to set Playing=false, etc
        }
    }
    
    if (XMLmgr->nodeHasThisTag(CUSTOM, sceneNum, _xmlnodei)) {
        string customclassname;
        customclassname = XMLmgr->getCustom(sceneNum,_xmlnodei);
        custom= new Custom(customclassname);
        //manager class "Custom" deals with all the possible classes
    }
    
    printf("Node %i completed\n",nodeNum);
}

void Node::assignDimensionsToHotspot() {
    //if this Node has an AREA, feed that to the hotspot
    //otherwise give it the dims of the visual asset

    //only one of these will be true
    //WIP just send all the sturct data? a picky detail
    if (picture!=NULL) {
        hotSpot->setDims(picture->dimensions, picture->centered);
    }
    else if (text!=NULL) {
        hotSpot->setDims(text->dimensions, text->centered);
    }
    else if (textcntnr!=NULL) {
        hotSpot->setDims(textcntnr->dimensions, textcntnr->centered);
    }//pm091814
    else if (video!=NULL) {
        hotSpot->setDims(video->dimensions, video->centered);
    }
    else if (animation!=NULL) {
        hotSpot->setDims(animation->dimensions, animation->centered);
    } else if (nodeIsBG) {
        hotSpot->setDims(ofGetWidth(), ofGetHeight());
    }
}

//called by Scene while building Nodes...
bool Node::isTheBG() {
    return nodeIsBG;
}
string Node::getBGinfo() {
    return BGinfo;
}

void Node::arrive(){
    if (audio!=NULL) { //this will be audio NOT assigned to a link/hotspot
        audio->start();
       //this is called always, but to keep asset paused/silent/invisible until some other event, let it decide what to do based on PLAYING...?
       //...or here say if (audioInfo.PLAYING=true) { audio->start(); }
    }
    if (video!=NULL) { video->start(); }
}
void Node::depart(){
    if (audio!=NULL) { audio->finish(); }
    if (video!=NULL) { video->finish(); }
}

void Node::update(ofPoint _mse) {
    if (audio!=NULL) { audio->update(); }
    
    if (canDrag) {
        nodeLoc = hotSpot->getLoc();
    }
    //NOTICE this Node design accepts only one type of visual media
    if (picture!=NULL) { picture->update(); }
    else if (text!=NULL) { text->update(); }
    else if (video!=NULL) { video->update(); }
    else if (animation!=NULL) { animation->update(); }
    else if (textcntnr!=NULL) { textcntnr->update(); } //pm091814
    
    if (custom!=NULL) { custom->update(); }
}


void Node::draw() {
    //NODE knows the locXY (get it from dragged HS if needed)
    
    if (custom!=NULL) { custom->draw(); } //WIP x,y is predefined in the obj for now
    
    if (hotSpot!=NULL) { nodeLoc = hotSpot->getLoc(); }
    
    if (picture!=NULL) { picture->draw(nodeLoc); }
    
    else if (text!=NULL) {
        text->draw(nodeLoc);
      /* //testing rollover alignment
        ofPushStyle();
        ofNoFill();
        ofRect(hotSpot->perimeter);
        //TODO: hot zone is UNDER the text!!
        ofPopStyle();
       */
    }
    else if (video!=NULL) { video->draw(nodeLoc); }
    else if (animation!=NULL) { animation->draw(nodeLoc); }
    else if (textcntnr!=NULL) { textcntnr->draw(nodeLoc); }
    //add playing of sound (once!) if so specified
}

//091814 when Node hears opencontainerevent...
void Node::openTxtCntnr(Events &e) {
    if (e.targetnodenum==nodeNum) {
        if  (nodeNum==105) {
            cout<<"look at this"<<endl;
        }
        if (textcntnr!=NULL) { textcntnr->setOpened(); }
    }
}
//091814 Scene might ask for this
string Node::getName(){
    return nodeName; //this is the name= property in XML
}

//091814 scene may call this from click on REVEAL link
void Node::setVisible() {
    if (textcntnr!=NULL) { textcntnr->setVisible(); }
    else if (text!=NULL) { text->setVisible(); }
//TODO: enable visible in these too...if ever
//    else if (video!=NULL) { video->setVisible(); }
//    else if (animation!=NULL) { animation->setVisible(); }
    else if (picture!=NULL) { picture->setVisible(); }
}

//092514
void Node::setLight() {
    if (textcntnr!=NULL) { textcntnr->setLight(); }
    else if (text!=NULL) { text->setLight(); }
}

//TODO: maybe make an AREA asset (not a node)
//Node::Node(ofVec4f _data, int _snum, int _hsnum, bool _centered) {
//    //_data format expected: x y width height
//    //_snum: scene number   _hsnum: assigned in Scene when creating
//
//    locX = _data[0];  locY=_data[1]; thisW=_data[2]; thisH=_data[3];
//
//    if (_centered) {
//        thisPerimeter.setFromCenter(locX,locY,thisW,thisH);
//    } else {
//        thisPerimeter.set(locX,locY,thisW,thisH);
//    }
//
//    thisClr = ofColor(255);
//
//    reset();
//
//    printf("Made AREA Node# %i\n",thisID);
//}

