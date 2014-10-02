//
//  Orbiter.cpp
//  xmlReadScene10ac
//
//  Created by psm on 4/12/14.
//
//  Used as a demo of having a "Custom" class named in the XML

#include "Orbiter.h"

Orbiter::Orbiter(float _x, float _y, float _dist, float _diam, ofColor _clr){
    //WIP decide where to specify these params
    x=_x;
    y=_y;
    diameter=_diam;
    distance=_dist;
    clr = _clr;
    orbitspeed = 2.0; //degrees per frame
    printf("Made an Orbiter\n");
}

void Orbiter::init( float _param, int _thisID ){
    
    
}

void Orbiter::update(){
    theta += orbitspeed;
    //distance+=0.1; //orbit slowly enlarges
}

void Orbiter::draw() {
    //printf("Orbiter is drawing.\n");
    ofPushMatrix();
    ofTranslate(x,y);
        ofPushMatrix();
            ofRotate(theta);
            ofTranslate(distance,0);
            ofPushStyle();
            ofFill();
            ofSetColor(clr);
            ofCircle(0,0,diameter);
            ofPopStyle();
        ofPopMatrix();
    ofPopMatrix();
}