#include "Game.h"

hlct::Game::Game(){
    // Asset pack
    imgPack.brand->load("game/helmet.png");
    imgPack.hero->load("game/hero.png");
    imgPack.liveFull->load("game/live-full.png");
    imgPack.liveEmpty->load("game/live-empty.png");
    imgPack.background->load("game/background.png");
    
    // Create a smaller version of brand helmet to use as bait
    ofImage baitImg;
    baitImg.setFromPixels(imgPack.brand->getPixels());
    baitImg.resize(baitImg.getWidth(), baitImg.getHeight());
    imgPack.bait->setFromPixels(baitImg.getPixels());
}

hlct::Game::~Game(){
    helmets.clear();
    
    scaleHero.removeListener(this, &Game::handleScaleHero);
    scaleBait.removeListener(this, &Game::handleScaleBait);
    scaleBaitWin.removeListener(this, &Game::handleScaleBaitWin);
    bUserExists.removeListener(this, &Game::handleUserExists);
    bUserPosing.removeListener(this, &Game::handlePosing);
    bStart.removeListener(this, &Game::handleGameStart);
    bAddHelmet.removeListener(this, &Game::handleAddHelmet);
}


void hlct::Game::setup(const ofRectangle& rect){
    
    this->resize(rect);
    setupInfoScreens(rect);
    
    helmets.clear();
    
    state = GAME_STATE_TITLE;
    
    ofRectangle stageRect(rect.getTopLeft(), rect.getWidth(), rect.getHeight());
    
    gameStartTimer.setDuration(HLCT_USER_POSE_DURATION);
    gameStartTimer.setCurve(LINEAR);
    
    gameTimer.setRepeatType(PLAY_ONCE);
    gameTimer.setRepeatTimes(0);
    gameTimer.setCurve(LINEAR);
    
    gameEndTimer.setDuration(HLCT_GAME_END_TO_TITLE_DURATION);
    gameEndTimer.setCurve(LINEAR);
    
    livesDisplay.setup(imgPack, HLCT_LIVES);
    
    params.setName("Game");
    
    ofParameterGroup stageParams;
    stageParams.setName("Stage");
    stageParams.add(bDebugStage.set("Stage Debug Mode", false));
    stageParams.add(stagePos.set("Top Left", ofVec2f(HLCT_CLAMP_STAGE, 0), ofVec2f::zero(), ofGetWindowRect().getBottomRight()));
    stageParams.add(stageWidth.set("Width", ofGetWidth() - HLCT_CLAMP_STAGE*2, ofGetWidth()/2, ofGetWidth()*2));
    stageParams.add(stageHeight.set("Height", ofGetHeight(), ofGetHeight()/2, ofGetHeight()*2));
    
    ofParameterGroup alignParams;
    alignParams.setName("Scaling and Alignment");
    alignParams.add(bDebugScaling.set("Debug Mode", false));
    alignParams.add(scaleHero.set("Hero Scale", 1.f, HLCT_HERO_SCALE_MIN, HLCT_HERO_SCALE_MAX));
    alignParams.add(scaleBait.set("Dropping Helmet Scale", .7f, HLCT_BAIT_SCALE_MIN, HLCT_BAIT_SCALE_MAX));
    alignParams.add(scaleBaitWin.set("Win Helmet Scale", .65f, HLCT_BAIT_SCALE_MIN, HLCT_BAIT_SCALE_MAX));
    alignParams.add(offsetBaitWin.set("Win Helmet Offset", 120, HLCT_BAIT_WIN_OFFSET_MIN, HLCT_BAIT_WIN_OFFSET_MAX));
    alignParams.add(diffBaitWin.set("Win Helmet Diff", .1f, HLCT_BAIT_WIN_DIFF_MIN, HLCT_BAIT_WIN_DIFF_MAX));
    alignParams.add(scaleLive.set("Live Icon Scale", 1.f, HLCT_LIVE_SCALE_MIN, HLCT_LIVE_SCALE_MAX));
    
    ofParameterGroup gameParams;
    gameParams.setName("Game Settings");
    gameParams.add(bStart.set("New", false));
    gameParams.add(endTime.set("Duration", 50, HLCT_MIN_DURATION, HLCT_MAX_DURATION));
    gameParams.add(currentTimeStr.set("Curent Time", "0"));
    gameParams.add(bPaused.set("Paused", false));
    
    params.add(stageParams);
    params.add(alignParams);
    params.add(gameParams);
    params.add(score.set("Score", 0, 0, HLCT_MAX_CATCH));
    
    params.add(bUserExists.set("User Exists", false));
    params.add(bUserPosing.set("User Posing", false));
    useOsc.set("Use Osc", true);
    bAddHelmet.set("Add Helmet", false);
    helmetSection.set("Helmet Section", 0, 0, HLCT_HELMET_SECTION_COUNT);
    
    scaleHero.addListener(this, &Game::handleScaleHero);
    scaleBait.addListener(this, &Game::handleScaleBait);
    scaleBaitWin.addListener(this, &Game::handleScaleBaitWin);
    bUserExists.addListener(this, &Game::handleUserExists);
    bUserPosing.addListener(this, &Game::handlePosing);
    bStart.addListener(this, &Game::handleGameStart);
    bAddHelmet.addListener(this, &Game::handleAddHelmet);
    
    receiver.setup(HLCT_OSC_PORT);
}


void hlct::Game::setupInfoScreens(const ofRectangle& rect){
    // Setup info screens and messages
    InfoScreen screenTitle;
    InfoScreen screenPosing;
    InfoScreen screenWin;
    InfoScreen screenLoose;
    vector<string> messages;
    
    // Title Screen
    string msg = "SAFETY FIRST!";
    messages.push_back(msg);
    msg = "COLLECT ALL HARD HATS!";
    messages.push_back(msg);
    screenTitle.setup(rect,
                      HLCT_INFO_SCREEN_DURATION,
                      imgPack.brand->getPixels(),
                      messages);
    
    // Posing Screen
    messages.clear();
    msg = "PLEASE WAIT IN RANGE...";
    messages.push_back(msg);
    screenPosing.setup(rect,
                       HLCT_INFO_SCREEN_DURATION,
                       imgPack.brand->getPixels(),
                       messages);
    
    // Win Screen
    messages.clear();
    msg = "YOU WIN!";
    messages.push_back(msg);
    msg = "YOU ARE IN SAFE NOW!";
    messages.push_back(msg);
    screenWin.setup(rect,
                    HLCT_INFO_SCREEN_DURATION,
                    imgPack.brand->getPixels(),
                    messages);
    
    // Loose Screen
    messages.clear();
    msg = "GAME OVER!";
    messages.push_back(msg);
    msg = "BE CAREFUL!";
    messages.push_back(msg);
    screenLoose.setup(rect,
                      HLCT_INFO_SCREEN_DURATION,
                      imgPack.brand->getPixels(),
                      messages);
    
    screens["title"] = screenTitle;
    screens["posing"] = screenPosing;
    screens["win"] = screenWin;
    screens["loose"] = screenLoose;
}


void hlct::Game::update(){
    
    ofRectangle stageRect(stagePos.get(), stageWidth.get(), stageHeight.get());
    
    if (useOsc){
        while (receiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            receiver.getNextMessage(m);
            
            // Set game duration
            if (m.getAddress() == HLCT_OSC_ENDPOINT_DURATION){
                endTime = m.getArgAsFloat(0);
            }
            if (m.getAddress() == HLCT_OSC_ENDPOINT_USER){
                bUserExists = m.getArgAsBool(0);
            }
            if (m.getAddress() == HLCT_OSC_ENDPOINT_POSING && bUserExists){
                bUserPosing = m.getArgAsBool(0);
            }
            if (m.getAddress() == HLCT_OSC_ENDPOINT_POSITION){
                float x = ofMap(m.getArgAsFloat(0), 0, 1, stageRect.getX(), stageRect.getX() + stageRect.getWidth());
                hero.moveTo(x);
            }
            if (m.getAddress() == HLCT_OSC_ENDPOINT_NEW){
                bStart = m.getArgAsBool(0);
            }
            if (m.getAddress() == HLCT_OSC_ENDPOINT_PAUSE){
                bPaused = m.getArgAsBool(0);
            }
        }
    }
    
    switch (state){
        case GAME_STATE_TITLE: {
            screens["title"].update(stageRect);
            break;
        }
            
        case GAME_STATE_POSING: {
            screens["posing"].update(stageRect);
            gameStartTimer.update(HLCT_ANIM_UPDATE_CYCLE);
            resizeLoadingBar(stageRect);
            break;
        }
            
        case GAME_STATE_GAME: {
            
            if (!bPaused){
                gameTimer.update(HLCT_ANIM_UPDATE_CYCLE);
                hero.update(stageRect, scaleHero);
                currentTime.set(gameTimer.getCurrentValue());
                currentTimeStr = ofToString(currentTime);
                
                bool gameEnded = gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH || livesLeft == 0;
                bool isDebugging = bDebugStage || bDebugScaling;
                if (!isDebugging){
                    if (gameEnded) {
                        endGame();
                    }
                }
                if (!gameEnded) {
                    if (helmets.size() == 0) {
                        addRandomHelmet();
                    }
                    ofRectangle heroRect;
                    heroRect.set(hero.getHitRectangle());
                    ofRectangle wRect(heroRect);
                    for (auto h : helmets){
                        h->update(stageRect, heroRect, scaleBait);
                    }
                    
                    int wi = 0;
                    for (auto wh : winHelmets){
                        wRect.setY(heroRect.getTop() - wh->getHeight() * diffBaitWin * wi + offsetBaitWin);
                        wh->update(stageRect, wRect, scaleBaitWin);
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
            resizeLoadingBar(stageRect);
            if (state == GAME_STATE_END_LOOSE){
                screens["loose"].update(stageRect);
            } else if (state == GAME_STATE_END_WIN){
                screens["win"].update(stageRect);
            }
            gameEndTimer.update(HLCT_ANIM_UPDATE_CYCLE);
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
    ofRectangle stageRect(stagePos.get(), stageWidth.get(), stageHeight.get());
    imgPack.background->draw(stageRect);
    
    switch (state){
        case GAME_STATE_TITLE:
            screens["title"].draw();
            break;
        case GAME_STATE_POSING: {
            screens["posing"].draw();
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
            hero.draw(bDebugScaling);
            for (auto h : winHelmets){
                h->draw(bDebugScaling);
            }
            for (auto h : helmets){
                h->draw(bDebugScaling);
            }
            livesDisplay.draw(stageRect, livesLeft, scaleLive);
            break;
        }
        case GAME_STATE_END_LOOSE:
            screens["loose"].draw();
            break;
        case GAME_STATE_END_WIN:
            screens["win"].draw();
            break;
        default:
            break;
    }
    
    // Draw end timer bar
    if (state == GAME_STATE_END_LOOSE || state == GAME_STATE_END_WIN){
        ofSetColor(ofColor::white, 50);
        drawLoadingBar(loadingBarRect, gameEndTimer.getCurrentValue());
        ofSetColor(ofColor::white);
    }
    
    if (bDebugStage) {
        ofPushStyle();
        ofSetColor(ofColor::greenYellow);
        ofNoFill();
        ofDrawRectangle(stageRect);
        ofPopStyle();
    }
}

void hlct::Game::drawLoadingBar(const ofRectangle& rect, const float& width){
    ofNoFill();
    ofDrawRectangle(rect);
    ofFill();
    ofDrawRectangle(rect.getTopLeft(),
                    rect.getWidth()*width,
                    rect.getHeight());
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
    
    ofRectangle stageRect(stagePos.get(), stageWidth.get(), stageHeight.get());
    hero.setup(ofVec2f(stageRect.getWidth()/2, stageRect.getBottom()), imgPack.hero->getPixels());
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
    helmet->setup(ofRectangle(stagePos.get(), stageWidth.get(), stageHeight.get()),
                  imgPack.bait->getPixels(), helmetSection);
    helmets.push_back(helmet);
}
