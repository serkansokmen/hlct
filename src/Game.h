#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"


#define HLCT_HELMET_SECTION_COUNT   4
#define HLCT_MAX_CATCH              10


namespace hlct {
    
    class Helmet {
        ofImage img;
        ofVec2f position;
        bool    alive;
        bool    touching;
        float   gravity;
        bool    win;
        ofRectangle intersectRect;
        
    public:
        void setup(const ofPixels& helmetPixels, const int& sectionIndex){
            
            gravity = ofRandom(-5.f, -4.f);
            float sectionWidth = ofGetWidth() / HLCT_HELMET_SECTION_COUNT;
            float sectionX = ofRandom(0, sectionWidth);
            position.x = sectionX + sectionWidth * sectionIndex;
            
            alive = true;
            img.setFromPixels(helmetPixels);
            float scl = ofClamp(ofRandomuf(), 0.6f, 1.f);
            img.resize(img.getWidth()*scl, img.getHeight()*scl);
            touching = false;
            
            intersectRect.set(position, img.getWidth(), img.getHeight());
        }
        void update(const ofVec2f& heroPos){
            if (alive && position.y <= ofGetHeight() - img.getHeight()) {
                if (!touching) {
                    position.y -= gravity;
                    intersectRect.set(position, img.getWidth(), img.getHeight());
                } else {
                    position.x = heroPos.x - img.getWidth()/2;
                    position.y = heroPos.y - img.getHeight()*1.5;
                }
            } else {
                alive = false;
            }
        }
        void draw(){
            img.draw(position);
        }
        bool isTouching(const ofVec2f& target){
            if (!win) {
                touching = intersectRect.inside(target);
                win = touching;
            }
            return touching;
        }
        bool isAlive() {
            return alive;
        }
        bool isWin() {
            return win;
        }
    };
    
    
    
    enum GameState {
        GAME_STATE_TITLE = 0,
        GAME_STATE_GAME = 1,
        GAME_STATE_END_WIN = 2,
        GAME_STATE_END_LOOSE = 3,
    };
    
    
    
    
    class GameAsset {
        ofImage title0, title1, win, loose;
        shared_ptr<ofImage> img;
    public:
        void setup(){
            title0.load("game/title_0.png");
            title1.load("game/title_1.png");
            win.load("game/win.png");
            loose.load("game/loose.png");
            img = shared_ptr<ofImage>(new ofImage);
        }
        void draw(const GameState& state){
            switch (state) {
                case GAME_STATE_TITLE: {
                    int seconds = (int)(ofGetElapsedTimeMillis() / 2000);
                    if (seconds % 2 == 0) {
                        *img = title0;
                    } else {
                        *img = title1;
                    }
                    break;
                }
                case GAME_STATE_END_WIN:
                    *img = win;
                    break;
                case GAME_STATE_END_LOOSE:
                    *img = loose;
                    break;
                default:
                    break;
            }
            
            if (state != GAME_STATE_GAME){
                img->draw(0, 0);
            }
        }
    };

    
    class Game {
        
        vector<shared_ptr<Helmet>>  helmets;
        vector<shared_ptr<Helmet>>  winHelmets;
        ofImage                     helmetImg;
        
        GameAsset                   gameAsset;
        GameState                   state;
        ofxAnimatableFloat          gameTimer;
        float                       startTime;
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
            helmets.clear();
            state = GAME_STATE_TITLE;
            
            params.setName("Game");
            params.add(endTime.set("End Time", 2, 2, 100));
            params.add(currentTime.set("Curent Time", 0.f, 0.f, 100.f));
            params.add(score.set("Score", 0, 0, HLCT_MAX_CATCH));
            params.add(helmetSection.set("Helmet Section", 0, 0, HLCT_HELMET_SECTION_COUNT));
        };
        
        void update(const ofVec2f& heroPos){
            switch (state) {
                case GAME_STATE_GAME: {
                    float dt = 1.f/60.f;
                    gameTimer.update(dt);
                    currentTime.set(gameTimer.getCurrentValue());
                    int bAddHelmet = (int)currentTime % 10 == 0;
                    
                    if (gameTimer.getCurrentValue() >= endTime || score == HLCT_MAX_CATCH){
                        endGame();
                    } else {
                        if (canAddHelmet()) {
                            addRandomHelmet();
                        }
                        for (auto h : helmets){
                            h->update(heroPos);
                        }
                        for (auto h : winHelmets){
                            h->update(heroPos);
                        }
                        for (int i=0; i<helmets.size(); ++i){
                            shared_ptr<Helmet> h = helmets[i];
                            if (!h->isAlive()) {
                                helmets.erase(helmets.begin() + i);
                            } else {
                                if (h->isTouching(heroPos)) {
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
            
            switch (state) {
                case GAME_STATE_GAME:
                    ofSetColor(ofColor::white);
                    for (auto h : helmets){
                        h->draw();
                    }
                    ofSetColor(ofColor::green);
                    for (auto h : winHelmets){
                        h->draw();
                    }
                    break;
                default:
                    break;
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
