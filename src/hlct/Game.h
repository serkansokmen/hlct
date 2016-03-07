#pragma once

#include "ofMain.h"
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
        ofImage                     helmetImg, helmetOutlineImg;
        
        GameAsset                   gameAsset;
        GameState                   state;
        LivesDisplay                livesDisplay;
        
        ofxAnimatableFloat          gameTimer;
        float                       startTime;
        int                         livesLeft;
        bool                        timerEnd;
        bool                        bTouchChecked;
        
        inline bool canAddHelmet(){
            return helmets.size() == 0;
        }
        
    public:
        
        Game(){};
        ~Game(){
            helmets.clear();
        };
        
        void setup(){
            
            gameAsset.setup();
            helmetImg.load("game/helmet.png");
            helmetOutlineImg.load("game/helmet_outline.png");
            helmets.clear();
            state = GAME_STATE_TITLE;
            
            params.setName("Game");
            params.add(endTime.set("End Time", 2, 2, 100));
            params.add(currentTime.set("Curent Time", 0.f, 0.f, 100.f));
            params.add(score.set("Score", 0, 0, HLCT_MAX_CATCH));
            params.add(helmetSection.set("Helmet Section", 0, 0, HLCT_HELMET_SECTION_COUNT));
            
            float x = ofGetWidth() - helmetOutlineImg.getWidth()*HLCT_LIVES*0.5 - 70;
            float y = 30;
            float w = helmetImg.getWidth()*0.5;
            float h = helmetImg.getHeight()*0.5;
            
            livesDisplay.setup(ofRectangle(x, y, w, h),
                               HLCT_LIVES,
                               "game/helmet.png",
                               "game/helmet_outline.png");
        };
        
        void update(const ofVec2f& heroPos){
            switch (state) {
                case GAME_STATE_GAME: {
                    float dt = 1.f/60.f;
                    gameTimer.update(dt);
                    currentTime.set(gameTimer.getCurrentValue());
                    
                    if (gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH || livesLeft == 0){
                        endGame();
                    } else {
                        if (canAddHelmet()) {
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
                for (auto h : helmets){
                    h->draw();
                }
                ofSetColor(ofColor::green);
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
            helmet.get()->setup(helmetImg.getPixels(), helmetSection);
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
    };
}
