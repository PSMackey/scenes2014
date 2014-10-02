#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "XMLmanager.h"
#include "Events.h" 
//here because ofNotifyEvent and ofAddListener are being used here

class Core : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
private:
    ofTrueTypeFont font10, font18;
    
    bool firstRun;
    
    ofPoint mseNow;
    vector<Scene*> scenes;
    int NUMOFSCENES;
    int currentScene;
    
    vector<string> scenenames;
    void goNextScene();
    
    void goToSceneName(Events &e);

    
    XMLmanager XMLmgr;
    
    void drawCustomCursor(bool _overhotspot);
    
};