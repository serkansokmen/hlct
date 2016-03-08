#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxAnimatableFloat.h"
#include "hlct/Helmet.h"
#include "hlct/GameState.h"
#include "hlct/GameAsset.h"
#include "hlct/LivesDisplay.h"
#include "Constants.h"


namespace hlct {    
    class Game {
        
        vector<shared_ptr<Helmet>>  helmets;
        vector<shared_ptr<Helmet>>  winHelmets;
        ofImage                     helmetImg, helmetWhiteImg, helmetOutlineImg;
        
        GameAsset                   gameAsset;
        GameState                   state;
        LivesDisplay                livesDisplay;
        
        ofxAnimatableFloat          gameTimer;
        ofVec2f                     heroPos;
        float                       startTime;
        int                         livesLeft;
        bool                        timerEnd;
        bool                        bTouchChecked;
        
        ofxOscReceiver              receiver;
        ofImage                     receivedImage;
        
    public:
        
        Game(){};
        ~Game(){
            helmets.clear();
        };
        
        void setup(){
            
            gameAsset.setup();
            helmetImg.load("game/helmet.png");
            helmetWhiteImg.load("game/helmet-white.png");
            helmetOutlineImg.load("game/helmet-outline.png");
            helmets.clear();
            state = GAME_STATE_TITLE;
            
            params.setName("Game");
            params.add(useOsc.set("Use OSC", false));
            params.add(endTime.set("End Time", 2, 2, 100));
            params.add(currentTime.set("Curent Time", 0.f, 0.f, 100.f));
            params.add(score.set("Score", 0, 0, HLCT_MAX_CATCH));
            params.add(helmetSection.set("Helmet Section", 0, 0, HLCT_HELMET_SECTION_COUNT));
            
            receiver.setup(OSC_PORT);
            
            float x = ofGetWidth() - helmetOutlineImg.getWidth()*HLCT_LIVES*0.5 - 70;
            float y = 30;
            float w = helmetImg.getWidth()*0.5;
            float h = helmetImg.getHeight()*0.5;
            
            livesDisplay.setup(ofRectangle(x, y, w, h),
                               HLCT_LIVES,
                               "game/helmet.png",
                               "game/helmet-outline.png");
        };
        
        void update(const ofVec2f& pos){
            
            heroPos.set(pos);
            
            switch (state) {
                case GAME_STATE_TITLE: {
                    while (receiver.hasWaitingMessages()){
                        ofxOscMessage m;
                        receiver.getNextMessage(m);

                        if (m.getAddress() == "/head/position"){
                            // both the arguments are int32's
                            float headX = m.getArgAsInt32(0);
                            float headY = m.getArgAsInt32(1);
                            ofLogWarning("/head/position " + ofToString(headX) + " : " + ofToString(headY));
                        }
                    }
                    break;
                }
                case GAME_STATE_GAME: {
                    
                    float dt = 1.f/60.f;
                    gameTimer.update(dt);
                    currentTime.set(gameTimer.getCurrentValue());
                    
                    while (receiver.hasWaitingMessages()){
                        ofxOscMessage m;
                        receiver.getNextMessage(m);
                        
                        if (m.getAddress() == "/image"){
                            ofBuffer buffer = m.getArgAsBlob(0);
                            receivedImage.load(buffer);
                        }
                    }
                    
                    if (gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH || livesLeft == 0){
                        endGame();
                    } else {
                        if (helmets.size() == 0) {
                            addRandomHelmet();
                        }
                        for (auto h : helmets){
                            h->update(heroPos);
                        }
                        int wi = 0;
                        for (auto h : winHelmets){
                            h->update(ofVec2f(heroPos.x, heroPos.y - h->getHeight()*0.25*wi));
                            wi++;
                        }
                        for (int i=0; i<helmets.size(); ++i){
                            shared_ptr<Helmet> h = helmets[i];
                            if (!h->isAlive()) {
                                helmets.erase(helmets.begin() + i);
                                livesLeft--;
                            } else {
                                if (h->isWin(heroPos)) {
                                    winHelmets.push_back(h);
                                    helmets.erase(helmets.begin() + i);
                                }
                            }
                        }
                        score = winHelmets.size();
                    }
                    break;
                }
                default:
                    break;
            }
        };
        
        void draw(){
            
            gameAsset.draw(state);
            
            if (state == GAME_STATE_GAME) {
                ofSetColor(ofColor::white);
                if (receivedImage.getWidth() > 0){
                    receivedImage.draw(heroPos);
                }
                for (auto h : helmets){
                    h->draw();
                }
                for (auto h : winHelmets){
                    h->draw();
                }
                livesDisplay.draw(livesLeft);
            }
        };
        
        void startGame(){
            
            gameTimer.setDuration(endTime);
            gameTimer.setRepeatType(PLAY_ONCE);
            gameTimer.setRepeatTimes(0);
            gameTimer.setCurve(LINEAR);
            
            helmets.clear();
            winHelmets.clear();
            addRandomHelmet();
            
            gameTimer.animateFromTo(0, endTime);
            state = GAME_STATE_GAME;
            score = 0;
            livesLeft = HLCT_LIVES;
            bTouchChecked = false;
        };
        
        void endGame(){
            timerEnd = true;
            startTime = ofGetElapsedTimeMillis();
            if (score < HLCT_MAX_CATCH) {
                state = GAME_STATE_END_LOOSE;
            } else {
                state = GAME_STATE_END_WIN;
            }
            helmets.clear();
        };
        
        void addRandomHelmet(){
            shared_ptr<Helmet> helmet = shared_ptr<Helmet>(new Helmet);
            helmetSection = (int)ofRandom(0, HLCT_HELMET_SECTION_COUNT);
            helmet.get()->setup(helmetWhiteImg.getPixels(), helmetSection);
            helmets.push_back(helmet);
        }
        
        bool isRunning(){
            return state == GAME_STATE_GAME;
        }
        
        ofParameterGroup    params;
        ofParameter<float>  currentTime;
        ofParameter<int>    endTime;
        ofParameter<int>    score;
        ofParameter<int>    helmetSection;
        ofParameter<bool>   useOsc;
    };
}
