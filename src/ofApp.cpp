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
    
    bgImg.load("game/background.png");
    
    ofParameterGroup params;
    game.setup();
    params.add(game.params);
    gui.setup(params);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
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
    
    bgImg.draw(0, 0);
    game.draw();
    
    if (game.isRunning()) {
        ofSetColor(ofColor::red);
        ofDrawCircle(game.getHeroPosition(), 20);
        ofSetColor(ofColor::white);
    }
    
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
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile("settings.xml");
}