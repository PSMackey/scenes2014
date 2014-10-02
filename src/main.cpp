#include "ofMain.h"
#include "Core.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1300,860, OF_WINDOW);	//see Scene for outline settings

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new Core());

}
