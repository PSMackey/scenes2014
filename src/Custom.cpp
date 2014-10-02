//
//  Custom.cpp
//  xmlReadScene10ac
//
//  Created by psm on 4/10/14.
//
//  Owns one of a collection of specialized objects

#include "Custom.h"

Custom::Custom(string _name){
    name=_name;
    printf("Made Custom class for %s\n",name.c_str());
    
    if (name=="orbiter") {
        //should allow XML to specify most/all of these params
        float _x=ofGetWidth()/2;
        float _y=ofGetHeight()/2;
        float _dist=75;
        float _diam=10;
        ofColor _clr= ofColor(190,200,0,255);
        orbiter=new Orbiter(_x,_y,_dist,_diam,_clr);
    }
}

//have a bunch of particular classes
//this class will instantiate the right one

void Custom::init( float _param, int _thisID ){
    
    
}
void Custom::update() {
    if (orbiter!=NULL) { orbiter->update(); }
}

void Custom::draw() {
    if (orbiter!=NULL) { orbiter->draw(); }
}