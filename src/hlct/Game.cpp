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
    
    bUserExists.removeListener(this, &Game::handleUserExists);
    bUserPosing.removeListener(this, &Game::handlePosing);
    bStart.removeListener(this, &Game::handleGameStart);
    bAddHelmet.removeListener(this, &Game::handleAddHelmet);
}


void hlct::Game::setup(const ofRectangle& rect){
    
    stageRect.set(rect);
    setupInfoScreens();
    
    helmets.clear();
    
    state = GAME_STATE_TITLE;
    
    hero.setup(ofVec2f(stageRect.getWidth()/2, stageRect.getHeight() - HLCT_HERO_BOTTOM),
               imgPack.hero->getPixels());
    
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
    
    livesDisplay.setup(imgPack, HLCT_LIVES);
    
    params.setName("Game");
    params.add(bStart.set("New Game", false));
    params.add(endTime.set("Game Duration", HLCT_MIN_DURATION, HLCT_MIN_DURATION, HLCT_MAX_DURATION));
    params.add(scaleHero.set("Hero Scale", HLCT_MAX_HERO_SCALE/2, HLCT_MIN_HERO_SCALE, HLCT_MAX_HERO_SCALE));
    params.add(scaleBait.set("Dropping Helmet Scale", HLCT_MAX_BAIT_SCALE/2, HLCT_MIN_BAIT_SCALE, HLCT_MAX_BAIT_SCALE));
    params.add(scaleBaitWin.set("Win Helmet Scale", HLCT_MAX_BAIT_SCALE/2, HLCT_MIN_BAIT_SCALE, HLCT_MAX_BAIT_SCALE));
    params.add(offsetBaitWin.set("Win Helmet Offset", 0, -200, 200));
    params.add(scaleLive.set("Live Icon Scale", HLCT_MAX_LIVE_SCALE/2, HLCT_MIN_LIVE_SCALE, HLCT_MAX_LIVE_SCALE));
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
}


void hlct::Game::setupInfoScreens(){
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
    screenTitle.setup(stageRect,
                      HLCT_INFO_SCREEN_DURATION,
                      imgPack.brand->getPixels(),
                      messages);
    
    // Posing Screen
    messages.clear();
    msg = "PLEASE WAIT IN RANGE...";
    messages.push_back(msg);
    screenPosing.setup(stageRect,
                       HLCT_INFO_SCREEN_DURATION,
                       imgPack.brand->getPixels(),
                       messages);
    
    // Win Screen
    messages.clear();
    msg = "YOU WIN!";
    messages.push_back(msg);
    msg = "YOU ARE IN SAFE NOW!";
    messages.push_back(msg);
    screenWin.setup(stageRect,
                    HLCT_INFO_SCREEN_DURATION,
                    imgPack.brand->getPixels(),
                    messages);
    
    // Loose Screen
    messages.clear();
    msg = "GAME OVER!";
    messages.push_back(msg);
    msg = "BE CAREFUL!";
    messages.push_back(msg);
    screenLoose.setup(stageRect,
                      HLCT_INFO_SCREEN_DURATION,
                      imgPack.brand->getPixels(),
                      messages);
    
    screens["title"] = screenTitle;
    screens["posing"] = screenPosing;
    screens["win"] = screenWin;
    screens["loose"] = screenLoose;
}


void hlct::Game::update(){
    
    float dt = 1.f/60.f;
    
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
                float x = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1, 0, stageRect.getWidth()), HLCT_CLAMP_STAGE, ofGetWidth()-HLCT_CLAMP_STAGE);
                hero.moveTo(ofVec2f(x, hero.getPosition().y));
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
            screens["title"].update();
            break;
        }
            
        case GAME_STATE_POSING:
            screens["posing"].update();
            gameStartTimer.update(dt);
            break;
            
        case GAME_STATE_GAME: {
            
            if (!bPaused){
                gameTimer.update(dt);
                hero.update(stageRect, scaleHero);
                currentTime.set(gameTimer.getCurrentValue());
                currentTimeStr = ofToString(currentTime);
                
                if (gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH || livesLeft == 0){
                    endGame();
                } else {
                    if (helmets.size() == 0) {
                        addRandomHelmet();
                    }
                    ofRectangle heroRect;
                    heroRect.set(hero.getRectangle());
                    ofRectangle wRect(heroRect);
                    for (auto h : helmets){
                        h->update(stageRect, heroRect, scaleBait);
                    }
                    
                    int wi = 0;
                    for (auto h : winHelmets){
                        wRect.setY(heroRect.getTop() - h->getHeight() * 0.15 * wi + offsetBaitWin);
                        h->update(stageRect, wRect, scaleBaitWin);
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
            if (state == GAME_STATE_END_LOOSE){
                screens["loose"].update();
            } else if (state == GAME_STATE_END_WIN){
                screens["win"].update();
            }
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
            hero.draw();
            for (auto h : winHelmets){
                h->draw();
            }
            for (auto h : helmets){
                h->draw();
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
    helmet->setup(stageRect, imgPack.bait->getPixels(), helmetSection);
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
