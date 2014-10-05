//
//  DataStructs.h
//  originally in xmlReadScene2
//  should match properties in XML per asset type
//  Created by psm on 2/15/14. Mods thru April 2014


#pragma once

//the possible asset types in a Node------------------------------

//091814
enum e_types {
    BG, PICT, VIDEO, ANIM, AREA, TEXT, TEXTCONTAINER, AUDIO, CUSTOM, LINK
};
//using this helps enforce standard terminology

//string version for talking to XML... see XMLmanager
//091814
const static int NTYPES=9;
const static string types[NTYPES] = {"BG", "PICT", "VIDEO", "ANIM", "AREA", "TEXT", "TEXTCONTAINER", "AUDIO", "CUSTOM"};
//these vars are available anywhere DataStructs.h is included

/* usage example, eg _tag = PICT
 XML.tagExists(types[_tag]); //recall that enums are actually ints 0,1,..
 //allows passing a legitimate string to the XML object
*/
//'static' is used to stop linker from finding more than one copy of this
//----------------------------------------------------------------

//092514
enum e_linkmodes {
    TRIGGER, REVEAL, OPEN, LIGHT, BRANCH, PLAY, TAKE
};
const static int NMODES=9;
const static string lnkmodes[NMODES] = {"TRIGGER","REVEAL", "OPEN", "LIGHT", "BRANCH", "PLAY", "TAKE"};

//data packets for delivering XML settings to particular assets of a node

struct sBGData {
    string FILE; //only FILE or CLR not both
    string CLR;
};

struct sLinkData {
    e_linkmodes MODE; //see e_linkmodes above
    string INFO;    //eg, scene name, which event, which media
};

struct sPictData {
    int X;
    int Y;
    string FILE;
    string ALIGN; //optional
    string DRAG; //optional
    float OUTRATE;  //fade-out speed 0.1 to 1.0
    float INRATE;   //fade-in " " "
};

struct sTextData {
    int X;
    int Y;
    int R;
    int G;
    int B;
    int R2, G2, B2;
    string FILE;
    string ALIGN; //optional
    string DRAG; //optional
    float OUTRATE;  //fade-out speed 0.1 to 1.0
    float INRATE;   //fade-in " " "
    bool VISIBLE;
};
//pm091814
struct sTextContainerData {
    int X;
    int Y;
    int R;
    int G;
    int B;
    int R2, G2, B2;
    string DISPNAME;  //displayed name of the container
    string ALIGN; //optional
    float OUTRATE;  //fade-out speed 0.1 to 1.0
    float INRATE;   //fade-in " " "
    bool VISIBLE;   //initially false, probably
    string CONTENTS;  // "|"-delimited list of what's inside
    //node will assume these are selectable
};


struct sVidData {
    int X;
    int Y;
    string FILE;
    string ALIGN;  //optional
    bool LOOP;     //optional, default F
    bool PLAYING;  //see note in aVideo::start  //optional
    float MAXVOL;  //optional
    bool SILENT;   //optional, default T
    bool DRAG;   //optional, default F
    float OUTRATE;  //fade-out speed 0.1 to 1.0
    float INRATE;   //fade-in " " "
};


struct sAnimData {
    int X;
    int Y;
    string IMGSDIR; //subdirectory of /data containing this img sequence
    string PREFIX; //start of each file name, eg, "squiggle" in the subdirectory Squiggle
    string EXTEN;  //eg. .png INCLUDE THE PERIOD
    bool ALIGN;    //optional
    string DRAG;   //optional
    //maybe add other params like speed, loop
};

struct sAudioData {
    string FILE;
    bool LOOP;      //default F
    bool PLAYING;   //default T
    float MAXVOL;   //default 1.0
    float PAN;      //default 0.0
    float OUTRATE;  //fade-out speed 0.1 to 1.0
    float INRATE;   //fade-in " " "
    //TODO: audio attached to [? in a Node with] a draggable element should pan L/R relative to X loc
    //TODO: consider multi-scene locations having one ambient sound
};

