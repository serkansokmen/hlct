#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_WARNING);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Helmet Catcher");
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    game.setup();
    bgImg.load("game/background.png");
    
    // Setup params
    gui.setName("Settings");
    ofParameterGroup params;
    params.add(game.params);
    
    gui.setup(params);
    
    btnStart.addListener(this, &ofApp::handleGameStart);
    btnAddHelmet.addListener(this, &ofApp::handleAddHelmet);
    
    gui.add(btnStart.setup("New Game"));
    gui.add(btnAddHelmet.setup("Add Random Helmet"));
    
    heroPosAnim = unique_ptr<ofxAnimatableOfPoint>(new ofxAnimatableOfPoint);
    heroPosAnim->setDuration(0.1);
    heroPosAnim->setPosition(ofVec2f(ofGetWidth()/2, ofGetHeight() - 240));
    heroPosAnim->setRepeatType(PLAY_ONCE);
    heroPosAnim->setRepeatTimes(0);
    heroPosAnim->setCurve(EASE_OUT);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float dt = 1.0f / 60.0f;
    heroPosAnim->update(dt);
    
    game.update(heroPosAnim->getCurrentPosition());
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
        ofDrawCircle(heroPosAnim->getCurrentPosition(), 20);
        ofSetColor(ofColor::white);
    }
    
    if (bDrawGui) {
        gui.draw();
    }
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
        handleGameStart();
    }
    if (key == '2') {
        handleAddHelmet();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (!game.useOsc) {
        heroPosAnim->animateTo(ofVec2f(x, heroPosAnim->getCurrentPosition().y));
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    btnStart.removeListener(this, &ofApp::handleGameStart);
    btnAddHelmet.removeListener(this, &ofApp::handleAddHelmet);
    
    gui.saveToFile("settings.xml");
}