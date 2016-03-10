#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_WARNING);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    ofxSmartFont::add(HLCT_INFO_SCREEN_FONT_PATH, HLCT_INFO_SCREEN_FONT_SIZE, HLCT_INFO_SCREEN_FONT_NAME);
    
    ofRectangle gameRect;
    gameRect.setFromCenter(ofGetWindowRect().getCenter(),
                           ofGetWidth() - HLCT_CLAMP_STAGE,
                           ofGetHeight());
    game.setup(gameRect);
//    if (settings.load("settings.xml")){
//        settings.deserialize(game.params);
//    }
    gui.setup(game.params);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    game.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    game.draw();
    
    if (bDrawGui) {
        gui.draw();
    }
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    game.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        bDrawGui = !bDrawGui;
    }
    if (key == 'f') {
        ofToggleFullscreen();
    }
    if (key == '1') {
        game.startGame();
    }
    if (key == '2') {
        game.addRandomHelmet();
    }
    if (key == 'O') {
        game.useOsc = !game.useOsc;
    }
    if (key == 'P') {
        game.bPaused = !game.bPaused;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::exit(){
//    settings.serialize(game.params);
//    settings.save("settings.xml");
    gui.saveToFile("settings.xml");
}