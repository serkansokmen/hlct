#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSmartFont.h"
#include "hlct/Game.h"


class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();

    void exit();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    hlct::Game          game;
    
    ofxPanel            gui;
    
    bool bDrawGui;
};
