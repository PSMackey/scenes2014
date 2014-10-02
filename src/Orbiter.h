//
//  Orbiter.h
//  xmlReadScene10ac
//
//  Created by psm on 4/12/14.


#pragma once

#include "ofMain.h"

class Orbiter {
    
public:
    Orbiter(float _x, float _y, float _dist, float _diam, ofColor _clr);
    
    void init( float _param, int _thisID );
    
    void update();
    void draw();
    
private:
    
    float param;
    int thisID;
    
    float x, y;
    float theta;
    float diameter;
    float distance;
    float orbitspeed;
    ofColor clr;
};