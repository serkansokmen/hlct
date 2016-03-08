#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "hlct/Game.h"


class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    ofImage             bgImg;
    hlct::Game          game;
    
    ofxPanel            gui;
    
    bool bDrawGui;
};
