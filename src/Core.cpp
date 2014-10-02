//xmlReadScene version 11    mackey Pratt DDA  April 2014
//WIP on a point&click interface engine
//scenes pm 1002 with parts by v.Yi
//forking for new/rev features

//TODO: make contents items inventoriable & puttable

///091814
///new hotspot event types added for text node control
///use events to tell a Scene to reveal/hide a node, show contents
///also added new asset type TEXTCONTAINER to hold a labelled collection of texts

//separately: tools for editing/verifying this XML scheme
// and authoring interfaces for asset placement & poly hotspot drawing

//SCENEs contain NODEs
//each NODE of a SCENE contains an asset, maybe a sound, and possibly a hotspot

#include "Core.h"

//DID implementing using a custom class via XML
//DID fade out rate  OUTRATE INRATE -- uses a sound object to play/fade sound -- mostly working [clickable hotspot ALSO has a basic sound play ability] need to test fadeOUT since scenes currently "cut" w/o delay

//BIG NEEDS:
//test layering (theory: order of nodes determines who gets the mouse event) TEST
//Reveals (rollovers) type of LINK? see revealsSmall3.xcodeproj
//Inventory
//Path History
/* mechanism for making decisions re what to show during gameplay, 
    eg how an object changes state after 1..n number of interactions
    eg an object from one scene now appears in another
*/
//Clock

//remembering states of a Scene when things have changed
//  during gameplay AND(!) saving to another xml for restarting

//You need a debugger for detecting faulty/malformed XML syntax [such as a separate app that reads the XML structure and just reprints what it "saw" with some slight formatting on screen or as a text file]
//Eventually you'll just be editing the XML directly, which makes having a spreadsheet only a starting-point [when converting from CSV to XML replace , with spaces so indents are visible]

/*NEXT BIG THINGS
    Quick startup screen with 'progress bar'
    Transitions
    Game > Rooms > Scenes > Nodes
    Organizing classes into project directory subfolders
 */


int hotspotactive; //"global"  updated by any Hotspot with mouseover, used for cursor control

//--------------------------------------------------------------
void Core::setup(){
    //pm ofSetFullscreen(true);
    ofBackground(255);
    ofSetFrameRate(60);
  //091814  CGDisplayHideCursor(NULL); //does this work? yes but is annoying during debug
    
    font10.loadFont("KeepCalm-Book.otf", 10);
    font18.loadFont("KeepCalm-Book.otf", 18);

    //error check
    //TODO: monitor error log for failed font and other media loads
    
    bool ok = XMLmgr.loadXMLfile("xmlSCENESsample11a.xml"); //xmlSCENESsample11a.xml//
    if (!ok) {
        printf("XML file load failed. You did spell that correctly, right?\n");
        //and stop everything else from happening, probably
    }
    
    firstRun=true;
    currentScene=-1;
    
    NUMOFSCENES = XMLmgr.getNumOfScenes();
    
    
    for (int i=0; i<NUMOFSCENES; i++) {
        printf("-------------------------------------------------\n");
        scenes.push_back(new Scene( &XMLmgr, &font18, i));
        //also store that Scene's name in array (same index as i, the "scene number")
        scenenames.push_back( scenes[i]->getName() );
    }

    //this event will be sent from any Hotspot
     ofAddListener(Events::scenechangeevent, this, &Core::goToSceneName);
    
    hotspotactive = 0; //scheme to determine if cursor is over any hotspot, set +1 by any mouseover in any aHotspot
}
//--------------------------------------------------------------
void Core::update(){
    mseNow.set(ofGetMouseX(), ofGetMouseY());
    if (!firstRun){ scenes[currentScene] -> update(mseNow); };
}

//--------------------------------------------------------------
void Core::goNextScene(){ //not currently enabled
    if (firstRun) { return; }
    
    //this is a simple "slide show"-like command
    if (currentScene==NUMOFSCENES-1) {
        currentScene=0;
    } else {
        currentScene++;
    }
}

//--------------------------------------------------------------
void Core::draw(){
    
    if (firstRun) {
        drawCustomCursor(hotspotactive!=0);
        hotspotactive=0; //reset for next frame
        
        ofBackground(255);
        ofSetColor(127);
        ofDrawBitmapString("TITLE SCREEN", ofGetWidth()/2.25, ofGetHeight()/2);
        //TODO: put up start screen immediately and show progressssss
    
    } else {
        scenes[currentScene] -> draw();
        
        drawCustomCursor(hotspotactive!=0);
        hotspotactive=0; //reset for next frame
        //any Hotspot will add 1 to this var if mouse is over it
    }
}

//--------------------------------------------------------------
//called from the event listener
void Core::goToSceneName(Events &e) {
    //look up the Link name in scenenames, use its index for goto
    //possibly inefficient way to search, but it works (name must be unique)...
    for (int i=0; i<NUMOFSCENES; i++) {
        
        if (scenenames[i]==e.nextscenename) {
            printf("Core::goToSceneName scenename %s\n",scenenames[i].c_str());
            
            //before specifying the new scene number, tell current scene you're leaving
            if (!firstRun) { scenes[currentScene]->depart(); }
            
            //TODO: Transition Mgr... do you wait for possible sound/picture fading out?
            currentScene=i;
            //also tell new scene you are arriving
            scenes[currentScene]->arrive();

            return; //done with the function, found a valid name, skip the message below
        }   
    }
    printf("Core::goToSceneName ? No scene named %s\n",e.nextscenename.c_str());
}

//--------------------------------------------------------------
//TODO: allow a variety of cursor types
void Core::drawCustomCursor(bool _overhotspot) {
    ofPushStyle();
    if (! _overhotspot) {
        ofSetLineWidth(1);
        ofNoFill();
        ofSetColor(255);
        ofCircle(ofGetMouseX(), ofGetMouseY(), 7);
        ofSetColor(0); //include some black for visibilty on light backgrounds
        ofCircle(ofGetMouseX(), ofGetMouseY(), 6);
    } else {
        //highlght the custom cursor
        ofSetColor(250);
        ofFill();
        ofCircle(ofGetMouseX(), ofGetMouseY(), 7);
        ofSetColor(0);
        ofNoFill();
        ofCircle(ofGetMouseX(), ofGetMouseY(), 7);
    }
    ofPopStyle();
}


//--------------------------------------------------------------
void Core::mousePressed(int x, int y, int button){
    if (firstRun) { return; }
        //use custom events to send info to any Listener...
        static Events anEvent;
        anEvent.mseloc = ofPoint(x,y);
        anEvent.msebttn = button;
        anEvent.currentscene = currentScene;
        //these are properties that are passed with the event
        ofNotifyEvent(Events::mousepressevent, anEvent);
        //mousepressevent is an event defined in Events.h
        //a Listener which hears this event is defined in the Hotspots


    
     //   scenes[currentScene] -> checkMousePressed(ofPoint(x,y));
}


//--------------------------------------------------------------
void Core::mouseReleased(int x, int y, int button){
    
    if (firstRun) {
        firstRun=false;
        currentScene=0;
        scenes[currentScene]->arrive();
    } else {
        
        static Events anEvent;
        anEvent.mseloc = ofPoint(x,y);
        anEvent.msebttn = button;
        anEvent.currentscene = currentScene;
        //these are properties that are passed with the event
        ofNotifyEvent(Events::mousereleaseevent, anEvent);
        //mousereleaseevent is an event defined in Events.h
        //a Listener which hears this event is defined in the Hotspots
    }
}

//--------------------------------------------------------------
void Core::keyPressed(int key){
    if (firstRun) { return; }
    
    //use custom events to send info to any Listener...
    static Events anEvent;
    anEvent.keycode = key;
    //keycode becomes a property (with the value of key) to pass with the event
    ofNotifyEvent(Events::keypressevent, anEvent);
    //keypressevent is an event defined in Events.h
}

//--------------------------------------------------------------
void Core::keyReleased(int key){
    
}

//--------------------------------------------------------------
void Core::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void Core::mouseDragged(int x, int y, int button){
    
}


//--------------------------------------------------------------
void Core::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void Core::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void Core::dragEvent(ofDragInfo dragInfo){
    
}