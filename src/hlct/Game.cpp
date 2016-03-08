#include "Game.h"

hlct::Game::~Game(){
    helmets.clear();
    
    bUserExists.removeListener(this, &Game::handleUserExists);
    bUserPosing.removeListener(this, &Game::handlePosing);
    bStart.removeListener(this, &Game::handleGameStart);
    bAddHelmet.removeListener(this, &Game::handleAddHelmet);
}


void hlct::Game::setup(const ofRectangle& rect){
    
    heroImg.load("game/hero.png");
    bgImg.load("game/background.png");
    helmetImg.load("game/helmet-red.png");
    helmetWhiteImg.load("game/helmet-white.png");
    helmetOutlineImg.load("game/helmet-outline.png");
    
    float scale = .5f;
    heroImg.resize(heroImg.getWidth()*scale, heroImg.getHeight()*scale);
    helmetWhiteImg.resize(helmetWhiteImg.getWidth()*scale, helmetWhiteImg.getHeight()*scale);
    
    stageRect.set(rect);
    gameAsset.setup(stageRect);
    helmets.clear();
    
    state = GAME_STATE_TITLE;
    
    heroPos = ofVec2f(stageRect.getWidth()/2, stageRect.getHeight() - HLCT_HERO_BOTTOM);
    
    gameStartTimer.setDuration(HLCT_USER_POSE_DURATION);
    gameStartTimer.setCurve(LINEAR);
    
    gameTimer.setRepeatType(PLAY_ONCE);
    gameTimer.setRepeatTimes(0);
    gameTimer.setCurve(LINEAR);
    
    gameEndTimer.setDuration(HLCT_GAME_END_TO_TITLE_DURATION);
    gameEndTimer.setCurve(LINEAR);
    
    loadingBarRect.setFromCenter(stageRect.getCenter().x,
                             stageRect.getCenter().y + 200,
                             400, 25);
    
    params.setName("Game");
    params.add(bStart.set("New Game", false));
    params.add(endTime.set("Game Duration", HLCT_MIN_DURATION, HLCT_MIN_DURATION, HLCT_MAX_DURATION));
    params.add(currentTimeStr.set("Curent Time", "0"));
    params.add(bPaused.set("Paused", false));
    
    params.add(score.set("Score", 0, 0, HLCT_MAX_CATCH));
    
    params.add(bUserExists.set("User Exists", false));
    params.add(bUserPosing.set("User Posing", false));
    useOsc.set("Use Osc", true);
    bAddHelmet.set("Add Helmet", false);
    helmetSection.set("Helmet Section", 0, 0, HLCT_HELMET_SECTION_COUNT);
    
    bUserExists.addListener(this, &Game::handleUserExists);
    bUserPosing.addListener(this, &Game::handlePosing);
    bStart.addListener(this, &Game::handleGameStart);
    bAddHelmet.addListener(this, &Game::handleAddHelmet);
    
    receiver.setup(HLCT_OSC_PORT);
    
    float x = stageRect.getWidth() - helmetOutlineImg.getWidth()*HLCT_LIVES*0.5 - 70;
    float y = 30;
    float w = helmetImg.getWidth()*0.5;
    float h = helmetImg.getHeight()*0.5;
    livesDisplay.setup(ofRectangle(x, y, w, h),
                       HLCT_LIVES,
                       "game/helmet-red.png",
                       "game/helmet-outline.png");
}


void hlct::Game::update(){
    
    float dt = 1.f/60.f;
    
    if (useOsc){
        while (receiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            receiver.getNextMessage(m);
            
            // Set game duration
            if (m.getAddress() == "/hlct/duration"){
                endTime = m.getArgAsFloat(0);
                ofLogWarning("/hlct/duration " + ofToString(endTime));
            }
            if (m.getAddress() == "/hlct/user"){
                bUserExists = m.getArgAsBool(0);
                ofLogWarning("/hlct/user " + ofToString(bUserExists));
            }
            if (m.getAddress() == "/hlct/posing" && bUserExists){
                bUserPosing = m.getArgAsBool(0);
                ofLogWarning("/hlct/posing " + ofToString(bUserPosing));
            }
            if (m.getAddress() == "/hlct/position"){
                float x = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1, 0, stageRect.getWidth()), HLCT_CLAMP_STAGE, ofGetWidth()-HLCT_CLAMP_STAGE);
                heroPos.x = x;
            }
            
            if (m.getAddress() == "/hlct/new"){
                bStart = m.getArgAsBool(0);
            }
            if (m.getAddress() == "/hlct/pause"){
                bPaused = m.getArgAsBool(0);
            }
        }
    }
    
    gameAsset.update(state);
    switch (state){
        case GAME_STATE_TITLE: {
            break;
        }
            
        case GAME_STATE_POSING:
            gameStartTimer.update(dt);
            break;
            
        case GAME_STATE_GAME: {
            
            if (!bPaused){
                gameTimer.update(dt);
                currentTime.set(gameTimer.getCurrentValue());
                currentTimeStr = ofToString(currentTime);
                
                if (gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH || livesLeft == 0){
                    endGame();
                } else {
                    if (helmets.size() == 0) {
                        addRandomHelmet();
                    }
                    ofRectangle heroRect(heroPos, heroImg.getWidth(), heroImg.getHeight());
                    ofRectangle wRect(heroRect);
                    for (auto h : helmets){
                        h->update(heroRect);
                    }
                    
                    int wi = 0;
                    for (auto h : winHelmets){
                        wRect.setY(heroRect.getTop() - h->getHeight()*0.15*wi + h->getHeight());
                        h->update(wRect);
                        wi++;
                    }
                    for (int i=0; i<helmets.size(); ++i){
                        shared_ptr<Helmet> h = helmets[i];
                        if (!h->isAlive()) {
                            helmets.erase(helmets.begin() + i);
                            livesLeft--;
                        } else {
                            if (h->isWin(heroRect)) {
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
        case GAME_STATE_END_LOOSE:
        case GAME_STATE_END_WIN: {
            gameEndTimer.update(dt);
            if (!gameEndTimer.isAnimating()){
                state = GAME_STATE_TITLE;
                bUserPosing = false;
                bUserExists = false;
            }
        }
            break;
        default:
            break;
    }
}


void hlct::Game::draw(){
    
    bgImg.draw(stageRect);
    
    switch (state){
        case GAME_STATE_TITLE:
            break;
        case GAME_STATE_POSING: {
            if (bUserExists && bUserPosing){
                ofSetColor(ofColor::white);
                drawLoadingBar(loadingBarRect, gameStartTimer.getCurrentValue());
                if (gameStartTimer.getCurrentValue() == 1.f && !gameStartTimer.isAnimating()){
                    startGame();
                }
            }
            break;
        }
        case GAME_STATE_GAME: {
            
            ofSetColor(ofColor::white);
            heroImg.draw(heroPos);
            for (auto h : winHelmets){
                h->draw();
            }
            for (auto h : helmets){
                h->draw();
            }
            livesDisplay.draw(livesLeft);
            break;
        }
        case GAME_STATE_END_LOOSE:
        case GAME_STATE_END_WIN: {
            ofSetColor(ofColor::white, 50);
            drawLoadingBar(loadingBarRect, gameEndTimer.getCurrentValue());
            ofSetColor(ofColor::white);
            break;
        }
        default:
            break;
    }
    
    if (state != GAME_STATE_GAME) {
        gameAsset.draw();
    }
}


void hlct::Game::startGame(){
    
    helmets.clear();
    winHelmets.clear();
    addRandomHelmet();
    
    gameTimer.setDuration(endTime);
    gameTimer.animateFromTo(0, endTime);
    state = GAME_STATE_GAME;
    currentTime = 0;
    currentTimeStr = "0";
    score = 0;
    livesLeft = HLCT_LIVES;
    bPaused = false;
}


void hlct::Game::endGame(){
    helmets.clear();
    timerEnd = true;
    startTime = ofGetElapsedTimeMillis();
    currentTime = 0;
    currentTimeStr = "0";
    
    if (score < HLCT_MAX_CATCH) {
        state = GAME_STATE_END_LOOSE;
    } else {
        state = GAME_STATE_END_WIN;
    }
    
    gameEndTimer.animateFromTo(0, 1);
}


void hlct::Game::addRandomHelmet(){
    shared_ptr<Helmet> helmet = shared_ptr<Helmet>(new Helmet);
    helmetSection = (int)ofRandom(0, HLCT_HELMET_SECTION_COUNT);
    helmet->setup(helmetWhiteImg.getPixels(), helmetSection, stageRect);
    helmets.push_back(helmet);
}

void hlct::Game::drawLoadingBar(const ofRectangle& rect, const float& width){
    ofNoFill();
    ofDrawRectangle(rect);
    ofFill();
    ofDrawRectangle(rect.getTopLeft(),
                    rect.getWidth()*width,
                    rect.getHeight());
}
