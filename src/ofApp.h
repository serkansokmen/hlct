#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxCameraSaveLoad.h"
#include "ofxAnimatableOfPoint.h"
#include "ColorTracker.h"
#include "Game.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y);
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void toggleGrabber(bool& yes);
    void togglePlayer(bool& yes);
    void toggleKinect(bool& yes);
    
    inline void handleGameStart(){
        game.startGame();
    }
    inline void handleAddHelmet(){
        game.addRandomHelmet();
    }
    inline bool isTracking() {
        return bUseGrabber || bUsePlayer || bUseKinect;
    }
    
    ofVideoGrabber          grabber;
    ofVideoPlayer           player;
    ofxKinect               kinect;
    unique_ptr<ofPixels>    trackPixels;
    tracker::ColorTracker   colorTracker;
    unique_ptr<ofxAnimatableOfPoint>     trackAveragePosAnim;
    
    hlct::Game              game;
    ofImage                 bgImg;
    
    ofxPanel                gui;
    ofxButton               btnStart;
    ofxButton               btnAddHelmet;
    ofParameter<bool>       bUseGrabber;
    ofParameter<bool>       bUsePlayer;
    ofParameter<bool>       bUseKinect;
    
    bool bDrawGui;
};
