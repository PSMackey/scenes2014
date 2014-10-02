//
//  Events.h
//  originally in sendMessage3Example
//
//  Created by psm on 3/5/13. Based on example by Todd Vanderlin on 2/25/12.
//  Mods Spring/Fall 2014



#pragma once
#include "ofMain.h"

class Events : public ofEventArgs {
    
public:
    
    Events();

    //event names [you define]
    static ofEvent <Events> mousereleaseevent;
    static ofEvent <Events> mousepressevent;
   
    
    //event properties [you define]
    //these are "sent with" the event message
    //and are read by the receiving Listener
    int currentscene;
    int msebttn;
    ofPoint mseloc;

    //these may be included in any of these event packages
    
    static ofEvent <Events> scenechangeevent;
    string nextscenename;
    

    static ofEvent <Events> keypressevent;
    //static ofEvent <Events> mouseroll;
    
    int      whichHotspot;
    string   message;
    int      keycode;
    
    int clicktype;
    
    //091814
    static ofEvent <Events> opencontainerevent;
    int targetnodenum;
    static ofEvent <Events> revealassetevent;
    int targetscenenum;
    string targetnodename;
    
    //092514
    static ofEvent <Events> lightswitchevent;
    string lightcolor;

};
