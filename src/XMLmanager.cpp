//
//  XMLmanager.cpp
//  originally in xml3scenesExample
//
//  Created by Peter Mackey on 14/02/14.
//
/*  RULES!
        No commas (if you are using .csv conversion from a speadsheet)
        Quotation marks not needed
        No spaces within attribute text (eg name=That Thing NO, name=ThatThing YES)
        Extra spaces for readbility such as indents is OK
 */
/*  NOTE: For optional values of each type, 
          the type's class may assign the default value
          ...but should include the default in the XML getValue, below
*/
//  SEE DataStructs.h for expected data fields

#include "XMLmanager.h"


XMLmanager::XMLmanager(){
    cout<<"XMLmanager created."<<endl;
}

bool XMLmanager::loadXMLfile(string _file){
    bool result=false;
    
    string s;
	if( XML.loadFile(_file) ){
		s = _file+" loaded\n";
        result=true;
	}else{
		s = "Unable to load "+_file+"  Check the bin/data/ folder. Look for funky syntax.";
        //if that happens, stop things from proceeding
	}
    cout<<s<<endl; //send message to the Console (alternative to printf)
    return result;
}

int XMLmanager::getNumOfScenes() {
    return XML.getNumTags("SCENE");
}

string XMLmanager::getSceneName(int _scenenum) {
    string s = XML.getAttribute("SCENE", "name", "", _scenenum);
    if (s=="") {
        cout<<"XMLmanager returns empty scene name. Big deal?"<<endl;
    } else {
        cout<<"XMLmanager returns scene named "<<s<<endl; }
    return s; //attribute is an 'inline' value (part of the XML tag) eg, <SCENE id="Home">
}
string XMLmanager::getNodeName(int _scenenum, int _nodenum) {
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag
    string s = XML.getAttribute("NODE", "name", "", _nodenum);
    if (s=="") s="nothing";
    cout<<"XMLmanager returns a Node named "<<s<<endl;
    XML.popTag();
    return s; //attribute is an 'inline' value (part of the XML tag) eg, <SCENE id=Home>
}
// ------------------------------------------------------------------
int XMLmanager::getNumNodes(int _scenenum) {
    //printf("XMLmanager::getNumNodes for _scenenum= %i\n",_scenenum);
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag
    int n = XML.getNumTags("NODE");
    XML.popTag(); //MAKE SURE push-pop are paired! (resets the root back to the xml document)
    printf("XMLmanager::getNumTags = %i for scenenum %i\n",n,_scenenum);
    return n;
}

//-----building assets for a Node--------------------------------------
int XMLmanager::getAssetCountInNode(int _scenenum, int _nodenum) {
    int n=0;
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _nodenum);
    
    for (int i=0; i<NTYPES; i++) {
        if (XML.tagExists(types[i],0)) { n++; }
    }
    //this may be entirely unnecessary
    XML.popTag();
    XML.popTag();
    printf("Found %i asset types in Node %i\n",n,_nodenum);
    return n;
}
bool XMLmanager::nodeHasThisTag(e_types _tag, int _scenenum, int _ni) {
    /* example
     <PICT>
     <FILE>door.jpg</FILE>
     <X>900</X>
     <Y>412</Y>
     </PICT>
     */
    bool does;
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _ni);
    does = XML.tagExists(types[_tag]); //'types' is declared in DataStructs.h
    //recall that enums are actually ints 0,1,..
    //doing this to pass a legitimate string to the XML object
    XML.popTag();
    XML.popTag();
    return does;
} //see below getNumTags()

bool XMLmanager::nodeHasLink(int _scenenum, int _ni) {
    bool does;
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _ni);
    does = XML.tagExists("LINK",0);
    XML.popTag();
    XML.popTag();
    return does;
}


//------------------------------------------------------------------------

string XMLmanager::getBG(int _scenenum, int _nodei) {
    /*
      <NODE>
        <BG><CLR>0</CLR></BG>
        or   <BG><FILE>bg-rock.jpg</FILE></BG>
      </NODE>
    */
    string BGvalue="";
    
    XML.pushTag("SCENE", _scenenum);
    
    XML.pushTag("NODE", _nodei);
    if (XML.tagExists("BG",0)) {
        //using 0 becuz only one tag like this is expected
        XML.pushTag("BG",0);
         if (XML.tagExists("FILE",0)) {
             BGvalue = XML.getValue("FILE", "", 0);
         } else {
             if (XML.tagExists("CLR",0)) {
                 BGvalue = XML.getValue("CLR", "", 0);
                 //just expecting a greyscale value for now
             }
         }
        XML.popTag();
    }
    XML.popTag();
    XML.popTag(); //MAKE SURE push-pop are paired!
    
    cout<<"XMLmanager::getBG returns BG as "<<BGvalue<<endl;
    return BGvalue;
    //probably should put some errorchecking in here
}



//int XMLmanager::getAlignment(int _scenenum) { //if the X-Y coords should be anchored at the center
//    return XML.getNumTags("SCENE[_scenenum]:BRANCH");
//} //should be inside AREA and IMG probably



//bool XMLmanager::hasAreaTag(int _scenenum, int _i) {
//    bool does;
//    XML.pushTag("SCENE", _scenenum);
//    XML.pushTag("BRANCH", _i); // _i in case there is more than one BRANCH in this scene
//    does = XML.tagExists("AREA");
//    XML.popTag();
//    XML.popTag();
//    return does;
//}


ofVec4f XMLmanager::getArea(int _scenenum, int _i) {
    printf("XMLmanager::getArea for _scenenum= %i\n",_scenenum);
    
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag
    XML.pushTag("BRANCH", _i);
    ofVec4f areadata;
    areadata[0]=ofToFloat(XML.getValue("AREA:X", "-1"));
    //using "-1" as a default value, to mean no XML data, useful for debugging
    areadata[1]=ofToFloat(XML.getValue("AREA:Y", "-1"));
    areadata[2]=ofToFloat(XML.getValue("AREA:W", "-1"));
    areadata[3]=ofToFloat(XML.getValue("AREA:H", "-1"));
    XML.popTag();
    XML.popTag(); //reset the root back to the xml document level
    return areadata;
}

sPictData XMLmanager::getPict(int _scenenum, int _ni) {
    printf("XMLmanager::getPict for _scenenum= %i node#%i\n",_scenenum,_ni);
    //this example assumes the PICT is inside a NODE, and ONLY ONE of them
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag //psm +1
    XML.pushTag("NODE", _ni);

    pictdata.X = ofToFloat(XML.getValue("PICT:X", "-1"));
    //"-1" is a default value, signifies no XML data useful for debugging
    pictdata.Y = ofToFloat(XML.getValue("PICT:Y", "-1"));
    pictdata.FILE = XML.getValue("PICT:FILE", "-1");
    //what happens if you dont have this tag?
    //vals below are optional, let aPicture class deal with default vals if ==""
    pictdata.ALIGN = XML.getValue("PICT:ALIGN","");
    pictdata.DRAG = XML.getValue("PICT:DRAG","");
    XML.popTag(); //reset the root back to the xml document
    XML.popTag(); //2 pushes = 2 pops, dont forget!
    return pictdata;
}

sTextData XMLmanager::getText(int _scenenum, int _ni) {
    printf("XMLmanager::getText for _scenenum= %i node#%i\n",_scenenum,_ni);
    //this example assumes the TEXT is inside a NODE, and ONLY ONE of them
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _ni);
    
    textdata.X = ofToFloat(XML.getValue("TEXT:X", "-1"));
    //"-1" is a default value, signifies no XML data useful for debugging
    textdata.Y = ofToFloat(XML.getValue("TEXT:Y", "-1"));
    //091714...
    textdata.R = ofToFloat(XML.getValue("TEXT:R", "-1"));
    textdata.G = ofToFloat(XML.getValue("TEXT:G", "-1"));
    textdata.B = ofToFloat(XML.getValue("TEXT:B", "-1"));
    textdata.R2 = ofToFloat(XML.getValue("TEXT:R2", "-1"));
    textdata.G2 = ofToFloat(XML.getValue("TEXT:G2", "-1"));
    textdata.B2 = ofToFloat(XML.getValue("TEXT:B2", "-1"));
    textdata.FILE = XML.getValue("TEXT:FILE", "-1");
    //what happens if you dont have this tag?
    //vals below are optional, let aPicture class deal with default vals if ==""
    textdata.ALIGN = XML.getValue("TEXT:ALIGN","");
    textdata.VISIBLE = (XML.getValue("TEXT:VISIBLE", "F")=="T") ? true:false;
    textdata.DRAG = XML.getValue("TEXT:DRAG","");
    XML.popTag(); //reset the root back to the xml document
    XML.popTag(); //2 pushes = 2 pops, dont forget!
    return textdata;
}

sTextContainerData XMLmanager::getTextContainer(int _scenenum, int _ni) {
    printf("XMLmanager::getTextContainer for _scenenum= %i node#%i\n",_scenenum,_ni);
    //this example assumes the TEXTCNTNR is inside a NODE, and ONLY ONE of them
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _ni);
    
    textcntnrdata.X = ofToFloat(XML.getValue("TEXTCONTAINER:X", "-1"));
    //"-1" is a default value, signifies no XML data useful for debugging
    textcntnrdata.Y = ofToFloat(XML.getValue("TEXTCONTAINER:Y", "-1"));
    //091714...
    textcntnrdata.R = ofToFloat(XML.getValue("TEXTCONTAINER:R", "-1"));
    textcntnrdata.G = ofToFloat(XML.getValue("TEXTCONTAINER:G", "-1"));
    textcntnrdata.B = ofToFloat(XML.getValue("TEXTCONTAINER:B", "-1"));
    textcntnrdata.R2 = ofToFloat(XML.getValue("TEXTCONTAINER:R2", "-1"));
    textcntnrdata.G2 = ofToFloat(XML.getValue("TEXTCONTAINER:G2", "-1"));
    textcntnrdata.B2 = ofToFloat(XML.getValue("TEXTCONTAINER:B2", "-1"));
    textcntnrdata.ALIGN = XML.getValue("TEXTCONTAINER:ALIGN","");
    textcntnrdata.DISPNAME = XML.getValue("TEXTCONTAINER:DISPNAME", "-1");
    textcntnrdata.VISIBLE = (XML.getValue("TEXTCONTAINER:VISIBLE", "F")=="T") ? true:false;    //watch if this is accurate syntax?
    textcntnrdata.CONTENTS = XML.getValue("TEXTCONTAINER:CONTENTS","");
    XML.popTag(); //reset the root back to the xml document
    XML.popTag(); //2 pushes = 2 pops, dont forget!
    return textcntnrdata;
}

sVidData XMLmanager::getVideo(int _scenenum, int _ni) {
    printf("XMLmanager::getVideo for _scenenum= %i node#%i\n",_scenenum,_ni);
    //assumes the VIDEO is inside a NODE
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _ni);
    XML.pushTag("VIDEO", 0);
    viddata.X = int(XML.getValue("X", 0.0, 0)); //property name, default val, 0 means only one of these tags at this level
    viddata.Y = int(XML.getValue("Y",  0.0, 0));
    viddata.FILE = XML.getValue("FILE", "", 0);
    viddata.ALIGN = XML.getValue("ALIGN","", 0);
    viddata.LOOP = (XML.getValue("LOOP", "F", 0)=="T") ? true:false;
    viddata.PLAYING = (XML.getValue("PLAYING", "T", 0)=="T") ? true:false;
    viddata.MAXVOL = XML.getValue("MAXVOL",1.0,0);
    viddata.SILENT = (XML.getValue("SILENT", "T", 0)=="T") ? true:false;
    viddata.DRAG = (XML.getValue("DRAG", "F", 0)=="T") ? true:false;
    viddata.INRATE = XML.getValue("INRATE", 0.0, 0);
    viddata.OUTRATE = XML.getValue("OUTRATE", 0.0, 0);
    XML.popTag();
    XML.popTag();
    XML.popTag();
    return viddata;
}

//091814 note this gets "asked" by a node if it thinks it has a Hotspot
sLinkData XMLmanager::getLink(int _scenenum, int _ni){
    //assumes the LINK tag is inside the NODE
    printf("XMLmanager::getLink for _scenenum= %i node#%i\n",_scenenum,_ni);
    string mode="";
    sLinkData linkdata;
    
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag
    XML.pushTag("NODE", _ni);
    
    mode=XML.getAttribute("LINK", "mode","", 0);
    //change data type so Node can use faster more consistent info
    if (mode=="TRIGGER"){       linkdata.MODE=TRIGGER; }
    else if (mode=="BRANCH"){   linkdata.MODE=BRANCH; }
    else if (mode=="PLAY"){     linkdata.MODE=PLAY; }
    //092514
    else if (mode=="LIGHT"){     linkdata.MODE=LIGHT; }
    //091814
    else if (mode=="REVEAL"){   linkdata.MODE=REVEAL; }
    else if (mode=="OPEN"){     linkdata.MODE=OPEN; }
    else if (mode=="TAKE"){     linkdata.MODE=TAKE; }
    
    linkdata.INFO=XML.getValue("LINK", "", 0); 
    
    XML.popTag();
    XML.popTag();
    return linkdata;
}

sAudioData XMLmanager::getAudio(int _sceneNum, int _ni){
    //assumes the AUDIO tag is inside a NODE
    sAudioData adata;
    printf("XMLmanager::getAudio scene=%i node=%i\n",_sceneNum, _ni);
    XML.pushTag("SCENE", _sceneNum);
    XML.pushTag("NODE", _ni);
    XML.pushTag("AUDIO", 0);
    adata.PAN = XML.getValue("PAN", 0.0, 0);
    adata.FILE = XML.getValue("FILE", "", 0);
    adata.LOOP = (XML.getValue("LOOP", "F", 0)=="T") ? true:false;
    adata.MAXVOL = XML.getValue("MAXVOL", 1.0, 0);
    adata.INRATE = XML.getValue("INRATE", 0.0, 0);
    adata.OUTRATE = XML.getValue("OUTRATE", 0.0, 0);
    adata.PLAYING = (XML.getValue("PLAYING", "T", 0)=="T") ? true:false;
    //see todo notes in DataStructs.h
    XML.popTag();
    XML.popTag();
    XML.popTag();
    return adata;
}

string XMLmanager::getAudioMode(int _scenenum, int _ni) {
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _ni);
    string s="";
    s = XML.getAttribute("AUDIO", "mode", "", 0);
    cout<<"XMLmanager AUDIO mode is "<<s<<endl; //can be empty
    XML.popTag();
    XML.popTag();
    return s; //attribute is an 'inline' value (part of the XML tag) eg, <SCENE id=Home>
}

//bool XMLmanager::hasAnimationTag(int _scenenum, int _i) {
//    bool does;
//    XML.pushTag("SCENE", _scenenum);
//    XML.pushTag("ANIM", _i); // _i in case there is more than one BRANCH in this scene
//    does = XML.tagExists("ANIM");
//    XML.popTag();
//    XML.popTag();
//    return does;
//}
int XMLmanager::getNumAnimations(int _scenenum, int _nodei) {
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag //psm +1
    XML.pushTag("NODE", _nodei);
    int n = XML.getNumTags("ANIM");
    XML.popTag();
    XML.popTag();
    if (n>0) printf("XMLmanager::gets NumAnimations = %i for _scenenum %i\n",n,_scenenum);
    return n;
}

sAnimData XMLmanager::getAnimation(int _scenenum, int _nodei) {
    printf("XMLmanager::getAnimation for _scenenum= %i\n",_scenenum);
    //TEMP this example assumes the IMG is inside a BRANCH (sometimes it isnt)
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular tag
    XML.pushTag("NODE", _nodei);
    animdata.X=ofToFloat(XML.getValue("ANIM:X", "-1"));
    //"-1" is default value to mean no XML data, useful for debugging
    animdata.Y=ofToFloat(XML.getValue("ANIM:Y", "-1"));
    animdata.IMGSDIR=XML.getValue("ANIM:IMGSDIR", "-1");
    animdata.PREFIX=XML.getValue("ANIM:PREFIX", "-1");
    
    //TODO: add a period if it is missing, be friendly!
    animdata.EXTEN=XML.getValue("ANIM:EXTEN", "-1"); //eg .png INCLUDE THE PERIOD

    animdata.ALIGN = (XML.getValue("ANIM:ALIGN","corner")=="corner") ? false:true;
    XML.popTag();
    XML.popTag(); //reset the root back to the xml document
    return animdata;
}

//how many tags of type _TAG in the Nodes of this Scene?
int XMLmanager::getNumTags(string _TAG, int _scenenum) {
    XML.pushTag("SCENE", _scenenum); //now looking inside this particular scene
    int nNodes = getNumNodes(_scenenum);
    //could report "no nodes in this scene" here
    int nTags=0;
    for (int n=0; n<nNodes; n++) {
        XML.pushTag("NODE", n);
        int nt = XML.getNumTags(_TAG);
        if (nt > 0) { //rare chance there are >1 ??
            nTags++;
        }
        XML.popTag();
    }
    XML.popTag(); //MAKE SURE push-pop are paired! (resets the root back to the xml document)
    if (nTags>0) printf("XMLmanager::getNumTags %s = %i\n",_TAG.c_str(), nTags);
    return nTags;
}

string XMLmanager::getCustom(int _scenenum, int _nodei) {
    string s;
    XML.pushTag("SCENE", _scenenum);
    XML.pushTag("NODE", _nodei);
    s = XML.getValue("CUSTOM", "", 0);
    XML.popTag();
    XML.popTag();
    return s;
    //a name of a class you want instantiated in this node
}

//=====================================================