#include "ofApp.h"
#include "ofxAssimpUtils.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_WARNING);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetWindowTitle("Bakarlar");
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    kinect.init();
    kinect.setRegistration(true);
    player.load("video.mov");
    trackPixels = unique_ptr<ofPixels>(new ofPixels);
    
    game.setup();
    bgImg.load("game/background.png");
    
    // Setup params
    gui.setName("Settings");
    ofParameterGroup params;
    params.add(bUseGrabber.set("Use Video Grabber", true));
    params.add(bUsePlayer.set("Use Video Player", false));
    params.add(bUseKinect.set("Use Kinect", false));
    params.add(colorTracker.params);
    params.add(game.params);
    
    gui.setup(params);
    
    btnStart.addListener(this, &ofApp::handleGameStart);
    btnAddHelmet.addListener(this, &ofApp::handleAddHelmet);
    
    gui.add(btnStart.setup("New Game"));
    gui.add(btnAddHelmet.setup("Add Random Helmet"));
    
    
    trackAveragePosAnim = unique_ptr<ofxAnimatableOfPoint>(new ofxAnimatableOfPoint);
    trackAveragePosAnim->setDuration(0.1);
    trackAveragePosAnim->setPosition(ofVec2f(ofGetWidth()/2, ofGetHeight() - 200));
    trackAveragePosAnim->setRepeatType(PLAY_ONCE);
    trackAveragePosAnim->setRepeatTimes(0);
    trackAveragePosAnim->setCurve(EASE_OUT);
    
    bUseGrabber.addListener(this, &ofApp::toggleGrabber);
    bUsePlayer.addListener(this, &ofApp::togglePlayer);
    bUseKinect.addListener(this, &ofApp::toggleKinect);
    
    gui.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float dt = 1.0f / 60.0f;
    trackAveragePosAnim->update(dt);
    
    game.update(trackAveragePosAnim->getCurrentPosition());
//    if (game.isRunning()){
//        game.checkHelmetTouch(trackAveragePosAnim->getCurrentPosition());
//    }
    
    if (bUseGrabber){
        grabber.update();
        if (grabber.isFrameNew()) {
            colorTracker.track(grabber.getPixels());
        }
    }
    if (bUsePlayer){
        player.update();
        if (player.isFrameNew()) {
            colorTracker.track(player.getPixels());
        }
    }
    if (bUseKinect){
        kinect.update();
        // there is a new frame and we are connected
        if (kinect.isFrameNew()) {
            colorTracker.track(kinect.getDepthPixels());
        }
    }
    
    if (isTracking()) {
        if (colorTracker.enabled){
            int contourCount = colorTracker.getBoundingRects().size();
            if (contourCount > 0 && isTracking()){
                
                unique_ptr<ofVec2f> average = unique_ptr<ofVec2f>(new ofVec2f);
                for (auto & rect : colorTracker.getBoundingRects()){
                    ofLogVerbose("Rect: " + ofToString(rect));
                    average->x += rect.x + rect.width/2;
                    average->y += rect.y + rect.height/2;
                }
                *average = *average / colorTracker.getBoundingRects().size();
                ofLogVerbose("Average: " + ofToString(*average));
                
                average->x = ofNormalize(average->x, 0, GRABBER_WIDTH) * ofGetWidth();
                average->y = ofNormalize(average->y, 0, GRABBER_HEIGHT) * ofGetHeight();
                ofLogVerbose("Mapped Average: " + ofToString(*average));
                if (!trackAveragePosAnim->isOrWillBeAnimating())
                    trackAveragePosAnim->animateTo(ofVec2f(average->x, trackAveragePosAnim->getCurrentPosition().y));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::toggleGrabber(bool& yes) {
    if (yes) {
        bUsePlayer = false;
        bUseKinect = false;
        grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    } else {
        if (grabber.isInitialized()) {
            grabber.close();
        }
    }
}

//--------------------------------------------------------------
void ofApp::togglePlayer(bool& yes) {
    if (yes) {
        bUseGrabber = false;
        bUseKinect = false;
        player.play();
    } else {
        player.stop();
    }
}

//--------------------------------------------------------------
void ofApp::toggleKinect(bool& yes) {
    if (yes) {
        bUseGrabber = false;
        bUsePlayer = false;
        kinect.init();
        kinect.setRegistration(true);
        kinect.open();
    } else {
        kinect.close();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    bgImg.draw(0, 0);
    if (isTracking()){
        colorTracker.draw();
    }
    game.draw();
    
    if (game.isRunning()) {
        ofSetColor(ofColor::red);
        ofDrawCircle(trackAveragePosAnim->getCurrentPosition(), 20);
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
    if (key == ' ') {
        player.setPaused(player.isPlaying());
    }
    if (key == '1') {
        handleGameStart();
    }
    if (key == '2') {
        handleAddHelmet();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofRectangle rect(colorTracker.drawPos.get(), GRABBER_WIDTH, GRABBER_HEIGHT);
    if (isTracking() && rect.inside(x, y)) {
        if (bUseGrabber) {
            *trackPixels = grabber.getPixels();
        } else if (bUsePlayer) {
            *trackPixels = player.getPixels();
        } else if (bUseKinect) {
            *trackPixels = kinect.getDepthPixels();
        };
        colorTracker.targetColor = trackPixels->getColor(x - colorTracker.drawPos->x, y - colorTracker.drawPos->y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (!isTracking()) {
        trackAveragePosAnim->animateTo(ofVec2f(x, trackAveragePosAnim->getCurrentPosition().y));
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    bUseGrabber.removeListener(this, &ofApp::toggleGrabber);
    bUsePlayer.removeListener(this, &ofApp::togglePlayer);
    bUseKinect.removeListener(this, &ofApp::toggleKinect);
    btnStart.removeListener(this, &ofApp::handleGameStart);
    btnAddHelmet.removeListener(this, &ofApp::handleAddHelmet);
    
    player.close();
    kinect.close();
    gui.saveToFile("settings.xml");
}