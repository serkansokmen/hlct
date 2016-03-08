#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_WARNING);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("HLCT");
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    ofParameterGroup params;
    ofRectangle gameRect;
    gameRect.setFromCenter(ofGetWindowRect().getCenter(),
                           ofGetWidth() - HLCT_CLAMP_STAGE,
                           ofGetHeight() - HLCT_CLAMP_STAGE);
    game.setup(gameRect);
    params.add(game.params);
    gui.setup(params);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    game.update();
//    if (game.isRunning()){
//        game.checkHelmetTouch(heroPosAnim->getCurrentPosition());
//    }
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
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile("settings.xml");
}