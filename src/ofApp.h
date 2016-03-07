#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAnimatableOfPoint.h"
#include "hlct/Game.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void toggleOsc(bool& yes);
    
    inline void handleGameStart(){
        game.startGame();
    }
    inline void handleAddHelmet(){
        game.addRandomHelmet();
    }
    
    unique_ptr<ofxAnimatableOfPoint>     heroPosAnim;
    
    hlct::Game              game;
    ofImage                 bgImg;
    
    ofxPanel                gui;
    ofxButton               btnStart;
    ofxButton               btnAddHelmet;
    ofParameter<bool>       bUseOsc;
    
    bool bDrawGui;
};
