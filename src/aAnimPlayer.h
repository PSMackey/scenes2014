/*
 *  aAnimPlayer.h

 *  Created by Mackey on 5/8/12. Mod 02/20/14
 *  Creates and owns an array of images
 *	Plays on command.
 */

#pragma once
#include "ofMain.h"
#include "DataStructs.h"

class aAnimPlayer {
    
public:
	aAnimPlayer(); //default constructor
	aAnimPlayer(sAnimData theAnimData);
	
	void	draw(ofPoint _nodeLoc);
    void	update();
    
    //asked for by Node, for possible Hotspot
    ofVec4f dimensions;
    bool    centered;
    
//    void    arrived();
//    void    departed();
	
private:
    sAnimData theAnimData;
    ofPoint     locXY;
    
    
    const int   INTERVAL = 500; //msecs //should be a property of sAnimData
    long long   startTime;
    void        startTimer();
    bool        timeHasExpired();
    void        watchAnimTimer();
    void        advanceTheFrame();


    int             numFrames;
	vector<ofImage>	sequence;
    int             frameCount;
	
	string      makePath(int i);
    
    int         getFileCountInDirectory();
    ofDirectory homedir;
    string      animimgsdirectory;
	string      imgfileprefix;
	string      imgfilesuffix;
	string      filename;
    
};