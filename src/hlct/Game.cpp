#include "Game.h"

hlct::Game::~Game(){
    helmets.clear();
    
    bStart.removeListener(this, &Game::handleGameStart);
    bAddHelmet.removeListener(this, &Game::handleAddHelmet);
}


void hlct::Game::setup(){
    
    gameAsset.setup();
    
    helmetImg.load("game/helmet.png");
    helmetWhiteImg.load("game/helmet-white.png");
    helmetOutlineImg.load("game/helmet-outline.png");
    helmets.clear();
    
    state = GAME_STATE_TITLE;
    
    heroPos.setDuration(0.1);
    heroPos.setPosition(ofVec2f(ofGetWidth()/2, ofGetHeight() - 240));
    heroPos.setRepeatType(PLAY_ONCE);
    heroPos.setRepeatTimes(0);
    heroPos.setCurve(EASE_OUT);
    
    params.setName("Game");
    params.add(bStart.set("New Game", false));
    params.add(useOsc.set("Osc", true));
    params.add(endTime.set("Game Duration", HLCT_MIN_DURATION, HLCT_MIN_DURATION, HLCT_MAX_DURATION));
    params.add(currentTimeStr.set("Curent Time", "0"));
    params.add(bPaused.set("Paused", false));
    params.add(bAddHelmet.set("Add Helmet", false));
    helmetSection.set("Helmet Section", 0, 0, HLCT_HELMET_SECTION_COUNT);
    params.add(score.set("Score", 0, 0, HLCT_MAX_CATCH));
    
    bStart.addListener(this, &Game::handleGameStart);
    bAddHelmet.addListener(this, &Game::handleAddHelmet);
    
    receiver.setup(HLCT_OSC_PORT);
    
    float x = ofGetWidth() - helmetOutlineImg.getWidth()*HLCT_LIVES*0.5 - 70;
    float y = 30;
    float w = helmetImg.getWidth()*0.5;
    float h = helmetImg.getHeight()*0.5;
    
    livesDisplay.setup(ofRectangle(x, y, w, h),
                       HLCT_LIVES,
                       "game/helmet.png",
                       "game/helmet-outline.png");
}

void hlct::Game::update(){
    
    float dt = 1.0f / 60.0f;
    heroPos.update(dt);
    
    while (receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if (m.getAddress() == "/hlct/position"){
            float x = ofMap(m.getArgAsFloat(1), 0, 1, 0, ofGetWidth());
            float y = ofMap(m.getArgAsFloat(0), 0, 1, 0, ofGetHeight());

            ofVec2f newPos(heroPos.getCurrentPosition());
            newPos.x = x;
            heroPos.animateTo(newPos);
        }
        
        if (m.getAddress() == "/hlct/duration"){
            endTime = m.getArgAsFloat(0);
            ofLogWarning("/hlct/duration " + ofToString(endTime));
        }
        
        if (m.getAddress() == "/hlct/new"){
            bStart = m.getArgAsBool(0);
        }
        if (m.getAddress() == "/hlct/pause"){
            bPaused = m.getArgAsBool(0);
        }
    }
    
    
    gameAsset.update(state);
    switch (state) {
        case GAME_STATE_TITLE: {
            break;
        }
        case GAME_STATE_GAME: {
            
            if (!bPaused){
                float dt = 1.f/60.f;
                gameTimer.update(dt);
                currentTime.set(gameTimer.getCurrentValue());
                currentTimeStr = ofToString(currentTime);
                
//                while (receiver.hasWaitingMessages()){
//                    ofxOscMessage m;
//                    receiver.getNextMessage(m);
//                    
//                    if (m.getAddress() == "/image"){
//                        ofBuffer buffer = m.getArgAsBlob(0);
//                        receivedImage.load(buffer);
//                    }
//                }
                
                if (gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH || livesLeft == 0){
                    endGame();
                } else {
                    if (helmets.size() == 0) {
                        addRandomHelmet();
                    }
                    for (auto h : helmets){
                        h->update(heroPos.getCurrentPosition());
                    }
                    int wi = 0;
                    for (auto h : winHelmets){
                        h->update(ofVec2f(heroPos.getCurrentPosition().x,
                                          heroPos.getCurrentPosition().y - h->getHeight()*0.25*wi));
                        wi++;
                    }
                    for (int i=0; i<helmets.size(); ++i){
                        shared_ptr<Helmet> h = helmets[i];
                        if (!h->isAlive()) {
                            helmets.erase(helmets.begin() + i);
                            livesLeft--;
                        } else {
                            if (h->isWin(heroPos.getCurrentPosition())) {
                                winHelmets.push_back(h);
                                helmets.erase(helmets.begin() + i);
                            }
                        }
                    }
                    score = winHelmets.size();
                }
            }
            
            break;
        }
        default:
            break;
    }
}

void hlct::Game::draw(){
    
    if (state == GAME_STATE_GAME) {
        ofSetColor(ofColor::white);
        if (receivedImage.getWidth() > 0){
            receivedImage.draw(heroPos.getCurrentPosition());
        }
        for (auto h : helmets){
            h->draw();
        }
        for (auto h : winHelmets){
            h->draw();
        }
        livesDisplay.draw(livesLeft);
    } else {
        gameAsset.draw();
    }
}

void hlct::Game::startGame(){
    
    gameTimer.setDuration(endTime);
    gameTimer.setRepeatType(PLAY_ONCE);
    gameTimer.setRepeatTimes(0);
    gameTimer.setCurve(LINEAR);
    
    helmets.clear();
    winHelmets.clear();
    addRandomHelmet();
    
    gameTimer.animateFromTo(0, endTime);
    state = GAME_STATE_GAME;
    currentTime = 0;
    currentTimeStr = "0";
    score = 0;
    livesLeft = HLCT_LIVES;
    bPaused = false;
}

void hlct::Game::endGame(){
    timerEnd = true;
    startTime = ofGetElapsedTimeMillis();
    currentTime = 0;
    currentTimeStr = "0";
    
    if (score < HLCT_MAX_CATCH) {
        state = GAME_STATE_END_LOOSE;
    } else {
        state = GAME_STATE_END_WIN;
    }
    helmets.clear();
}

void hlct::Game::addRandomHelmet(){
    shared_ptr<Helmet> helmet = shared_ptr<Helmet>(new Helmet);
    helmetSection = (int)ofRandom(0, HLCT_HELMET_SECTION_COUNT);
    helmet.get()->setup(helmetWhiteImg.getPixels(), helmetSection);
    helmets.push_back(helmet);
}

