//
//  aHotspot.cpp
//
//  Created by psm Feb-Mar 2014.

//  Owned by a Node with a Link
//  Acquires the hot zone from the visual asset (default)
//  or, if defined, a polygonal area

//TODO: include algorithm for alpha-transparency detection
//TODO: implement mode=ROLLOVER to Reveal 'itself'... or who else?

#include "aHotspot.h"
#include "aText.h"

extern int hotspotactive; //used by Core to determine cursor display
extern int winX; //defined in Scene
extern int winY;

void aHotspot::registerAsset(aText* _assetptr) { //092614 Hotspot "knows about" its Asset TEXT only
    myAssetPtr=_assetptr;
    myAssetsName=myAssetPtr->getName(); //only used for "TAKEN", only applies to aText assets
}

aHotspot::aHotspot(){}

//version for a Node with a Link
aHotspot::aHotspot(sLinkData _LD, int _nodenum, int _scenenum, string _scname){
//    winW = 1200;
//    winH = 660;
//    winX = (ofGetWidth()/2)-(winW/2);
//    winY = (ofGetHeight()/2)-((winH+100)/2);
    
    mySceneName = _scname; //not using
    mySceneNum = _scenenum;
    myNodeNum = _nodenum;
    
    /* 091814 note if this hotspot's Node contains a TextContainer asset,
     link MODE should be OPEN (used on mouseReleased)
     and INFO part can say anything (it is just a meaningless placeholder)
     example  <LINK mode=OPEN> hoohah </LINK>
     */
    myLinkType = _LD.MODE;
    myLinkInfoStr = _LD.INFO;
    draggable = false;
    //TODO: enable setting this visibility flag from Asset(! done ?)
    visible=true;
    
    // listen for custom events from Core
    ofAddListener(Events::mousepressevent, this, &aHotspot::mousePressed);
    ofAddListener(Events::mousereleaseevent, this, &aHotspot::mouseReleased);
    string t = lnkmodes[myLinkType];
    printf("Made aHotspot with a link for nodenum %i\n",myNodeNum);
}

aHotspot::aHotspot(ofVec4f _dims, bool _centered, int _id, int _nodenum, int _scenenum){
    mySceneNum = _scenenum;
    myNodeNum = _nodenum;
    idnum = _id;
    setDims(_dims, _centered);
    myLinkType = TAKE; //<----------------
    draggable = false;
    
    visible=true; //091814 assume true, set this false from asset if not
    
    // listen for custom events from Core
    ofAddListener(Events::mousepressevent, this, &aHotspot::mousePressed);
    ofAddListener(Events::mousereleaseevent, this, &aHotspot::mouseReleased);
    string t = lnkmodes[myLinkType];
    printf("! Made aHotspot in nodenum %i with a link of type %s for a 'takable' item# %i\n",myNodeNum, t.c_str(),idnum);
}

//version for a non-link Node that wants to be draggable
aHotspot::aHotspot(int _nodenum, int _scenenum){
    mySceneNum = _scenenum;
    myNodeNum = _nodenum;
    draggable = true;
    visible=true; //091814 assume true, set this false from asset if not
    // listen for custom events from Core
    ofAddListener(Events::mousepressevent, this, &aHotspot::mousePressed);
    ofAddListener(Events::mousereleaseevent, this, &aHotspot::mouseReleased);
    printf("Made draggable aHotspot for nodenum %i\n",myNodeNum);
}

//from the Node
void aHotspot::setAudioPlayOnClick(string _file, bool _loop){
    if (audio==NULL) { audio = new ofSoundPlayer(); }
    audio->loadSound(_file);
    audio->setLoop(_loop);
    //sound->setMultiplay(true); //if multiple sounds arent playing simultaneously
}


//also called from Node during inititalization
void aHotspot::setDims(ofVec4f _dims, bool _centered) {
    locXY.x = _dims[0];
    locXY.y = _dims[1];
    W = _dims[2];
    H = _dims[3];
    centered = _centered;
    perimeter.set(winX+locXY.x, winY+locXY.y-H, W, H);
    wasEntered=false;
    //TODO: not using alignment yet, adjust perim(?) for centered==TRUE
}
void aHotspot::setDims(int _w, int _h) { //the BG case
    locXY.x = 0;
    locXY.y = 0;
    W = _w;
    H = _h;
    centered = false;
    perimeter.set(locXY.x, locXY.y, W, H);
}
//TODO: allow a polygonal area to be set

//-------------------------------------------------------------
ofPoint aHotspot::getLoc() {
    return locXY;
}

void aHotspot::setVisible(bool _torf){ //from my Asset
    visible=_torf;
}
//-------------------------------------------------------------
bool aHotspot::update() {
    mouseIsOver=false; //reset for next check
    
    if (draggable && visible) {
        //TODO: WIP dont! capture mouse location if it 'happens' to be over this as the scene begins
        perimeter.set(locXY.x, locXY.y, W, H); //keep updating in case dragging
        if (mouseIsDown) { //user may be trying to drag
            ofPoint mse = ofPoint(ofGetMouseX(),ofGetMouseY());
            ofVec2f newloc = mse - mseDnOffset;
            
            if (!(newloc.match(locXY,0.1))) { //started dragging
                //perimeter needs to follow the new position
                perimeter.set(newloc.x, newloc.y, W, H);
                locXY.set(newloc);
            }
        }
    }
    //moved from checkmouseover //092614
    if (visible) { //091814
        //probably not sensitive to layer priority (underlying hotspots may also respond)
        mouseIsOver = perimeter.inside(ofGetMouseX(), ofGetMouseY());
        //if (mouseIsOver) printf("Mouse is OVER aHotspot of Node# %i\n",myNodeNum);
        
        //did this "enter" just happen? may need in future
        if ( !wasEntered && mouseIsOver ) {
            wasEntered = true;
            //return true; EnterEVENT!
            //myNodePtr->mouseJustEntered();
        } else {
            if ( wasEntered && !mouseIsOver ) {
                wasEntered = false;
                //return true;  LeaveEVENT!
            }
        }
        
        if ( mouseIsOver) { hotspotactive++; } //used by Core to determine cursor display
        
    } //end visible
    
    return mouseIsOver;
}

void aHotspot::mousePressed(Events &e) {
    if (visible) { //091814
        int _x= e.mseloc.x;
        int _y= e.mseloc.y;
        int button = e.msebttn;
        
        if (e.currentscene==mySceneNum) {
            if (mouseIsOver) {
                mseDnOffset = ofVec2f(_x - locXY.x, _y - locXY.y);
                mouseIsDown = true;
                //printf("Node %i mouseIsDown = true\n",myNodeNum);
                if (audio!=NULL) { audio->play(); }
                //TODO: test audio play on click, does it reset?
            } else {
                mouseIsDown = false;
            }
        }
    } //end visible
}

void aHotspot::mouseReleased(Events &e){
    if ((e.currentscene==mySceneNum)&&mouseIsOver) { //091814
        if (visible) {
            mouseIsDown = false;
            printf("That's a click on aHotspot %s in Node #%i of Scene#%i %s\n",myAssetsName.c_str(),myNodeNum,mySceneNum,mySceneName.c_str()); //092214
            //pm
            if (myNodeNum==105) {
                cout<<"look at this"<<endl;
            }
            
            static Events eventinfo; //used by ofNotifyEvent's
        
            switch (myLinkType) {
                case LIGHT: //092514
                    printf("A Hotspot tries LIGHT\n");
                    eventinfo.lightcolor = myLinkInfoStr;
                    eventinfo.targetscenenum = mySceneNum;
                    ofNotifyEvent(Events::lightswitchevent, eventinfo);
                    break;
                    
                case REVEAL: //make something visible, toggles to invisible
                    printf("A Hotspot tries REVEAL\n");
                    //so send revealassetevent to Scenes...
                    eventinfo.targetnodename = myLinkInfoStr;
                    eventinfo.targetscenenum = mySceneNum; //to confirm correct Scene
                    ofNotifyEvent(Events::revealassetevent, eventinfo);
                    //the Listener which hears this event is defined in Scene
                    break;
                
                case OPEN:
                    //forces TEXTCONTAINER to list (or hide) its contents
                    //tell my node to draw the list of contents
                    printf("Hotspot sending opencontainerevent with this info: %s\n",myLinkInfoStr.c_str());
                
                    eventinfo.targetnodenum = myNodeNum; //to confirm correct Node
                    ofNotifyEvent(Events::opencontainerevent, eventinfo);
                    //the Listener which hears this event is defined in Node
                    //(if that node contains the TEXTCONTAINER asset type)
                
                    break;
                
                case TAKE:  //092614  see approx line 54 above
                    printf("...specifically, a TAKE on item %i\n",idnum);
                    //tell your asset it has been TAKEN
                    myAssetPtr->setTaken(); //yay!
                    break;
                
                case TRIGGER:
                    //tbd
                    break;
                case BRANCH:
                    //eventually, more options
                    changeScene();
                    break;
                case PLAY:
                    //tbd
                    break;
                default:
                    break;
            }
        }
    //TODO: specify a particular cursor (need that info in Core)
    } //end visible
}

void aHotspot::changeScene() {
        printf("Hotspot sending scenechangeevent with this info: %s\n",myLinkInfoStr.c_str());
        static Events anEvent;
        anEvent.nextscenename = myLinkInfoStr;
        ofNotifyEvent(Events::scenechangeevent, anEvent);
        //a Listener which hears this event is defined in Core
}
