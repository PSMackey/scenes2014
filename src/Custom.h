//
//  Custom
//  xmlReadScene10ac
//
//  Created by psm on 4/10/14.
//
//

#pragma once

#include "ofMain.h"
#include "Orbiter.h"

class Custom {
    
public:
    Custom(string _name);
    
    void init( float _param, int _thisID );
    void update();
    void draw();
    
private:
    string name;
    
    float param;
    int thisID;
    Orbiter* orbiter=NULL;
    
};