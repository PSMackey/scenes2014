//
//  Events.cpp


#include "Events.h"

Events::Events(){}

ofEvent <Events> Events::keypressevent;

ofEvent <Events> Events::mousepressevent;

ofEvent <Events> Events::mousereleaseevent;

ofEvent <Events> Events::scenechangeevent;

//091814
ofEvent <Events> Events::opencontainerevent;
ofEvent <Events> Events::revealassetevent;
//092514
ofEvent <Events> Events::lightswitchevent;