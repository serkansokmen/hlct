#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"


namespace hlct {
    
    class Helmet {
        ofImage img;
        ofVec2f position;
        bool    alive;
        bool    touching;
        float   gravity;
        ofRectangle intersectRect;
        
    public:
        void setup(const ofPixels& helmetPixels){
            gravity = ofRandom(-5.f, -4.f);
            position.x = ofRandomWidth();
            alive = true;
            img.setFromPixels(helmetPixels);
            float scl = ofClamp(ofRandomuf(), 0.6f, 1.f);
            img.resize(img.getWidth()*scl, img.getHeight()*scl);
            touching = false;
            
            intersectRect.set(position, img.getWidth(), img.getHeight());
        }
        void update(){
            if (alive && position.y <= ofGetHeight() - img.getHeight()) {
                if (!touching) {
                    position.y -= gravity;
                    intersectRect.set(position, img.getWidth(), img.getHeight());
                }
            } else {
                alive = false;
            }
        }
        void draw(){
            if (position.y <= ofGetHeight() - img.getHeight()) {
                if (touching)
                    ofSetColor(ofColor::green);
                else
                    ofSetColor(ofColor::white);
            } else {
                ofSetColor(ofColor::red);
            }
            img.draw(position);
        }
        bool checkTargetIntersects(const ofVec2f& target){
            if (!touching) {
                touching = intersectRect.inside(target);
            }
            return touching;
        }
        bool isAlive() {
            return alive;
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
                case GAME_STATE_GAME:
                    break;
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
        ofImage                     helmetImg;
        
        GameAsset                   gameAsset;
        GameState                   state;
        ofxAnimatableFloat          gameTimer;
        float                       startTime;
        bool                        timerEnd;
        
        inline bool helmetExists(){
            bool exists = false;
            if (helmets.size() < 1) {
                return exists;
            } else {
                bool yes = false;
                for (auto h : helmets) {
                    if (!yes)
                        yes = h.get()->isAlive();
                }
                exists = yes;
            }
            return exists;
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
            params.add(endTime.set("End Time", 2, 2, 25));
            params.add(currentTime.set("Curent Time", 0.f, 0.f, 25.f));
            params.add(score.set("Score", 0, 0, 3));
        };
        
        void update(const ofVec2f& heroPos){
            switch (state) {
                case GAME_STATE_TITLE:
                    break;
                case GAME_STATE_GAME: {
                    float dt = 1.f/60.f;
                    gameTimer.update(dt);
                    currentTime.set(gameTimer.getCurrentValue());
                    
                    if (gameTimer.getCurrentValue() >= endTime || score == 3){
                        endGame();
                    } else {
                        if (!helmetExists()) {
                            addRandomHelmet();
                        }
                        int wins = 0;
                        for (int i=0; i<helmets.size(); ++i){
                            shared_ptr<Helmet> h = helmets[i];
                            if (h->checkTargetIntersects(heroPos)) {
                                wins++;
                            }
                            
                            if (h.get()->isAlive()){
                                h.get()->update();
                            } else {
                                helmets.erase(helmets.begin() + i);
                            }
                        }
                        score = wins;
                    }
                    break;
                }
                case GAME_STATE_END_WIN:
                    break;
                case GAME_STATE_END_LOOSE:
                    break;
                default:
                    break;
            }
        };
        
        void draw(){
            
            gameAsset.draw(state);
            
            switch (state) {
                case GAME_STATE_TITLE:
                    break;
                case GAME_STATE_GAME:
                    for (auto h : helmets){
                        h.get()->draw();
                    }
                    break;
                case GAME_STATE_END_WIN:
                    break;
                case GAME_STATE_END_LOOSE:
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
            addRandomHelmet();
            
            gameTimer.animateFromTo(0, endTime);
            state = GAME_STATE_GAME;
            score = 0;
        };
        
        void endGame(){
            timerEnd = true;
            startTime = ofGetElapsedTimeMillis();
            if (score < 3) {
                state = GAME_STATE_END_LOOSE;
            } else {
                state = GAME_STATE_END_WIN;
            }
            helmets.clear();
        };
        
        void addRandomHelmet(){
            shared_ptr<Helmet> helmet = shared_ptr<Helmet>(new Helmet);
            helmet.get()->setup(helmetImg.getPixels());
            helmets.push_back(helmet);
        }
        
        bool isRunning(){
            return state == GAME_STATE_GAME;
        }
        
        ofParameterGroup    params;
        ofParameter<float>  currentTime;
        ofParameter<int>    endTime;
        ofParameter<int>    score;
    };
    
}
