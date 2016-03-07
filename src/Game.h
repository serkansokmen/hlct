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
        
    public:
        void setup(const ofPixels& helmetPixels){
            gravity = ofRandom(-5.f, -4.f);
            position.x = ofRandomWidth();
            alive = true;
            img.setFromPixels(helmetPixels);
            float scl = ofClamp(ofRandomuf(), 0.2f, 0.8f);
            img.resize(img.getWidth()*scl, img.getHeight()*scl);
            touching = false;
        }
        void update(){
            if (alive && position.y <= ofGetHeight() - img.getHeight()) {
                if (!touching)
                    position.y -= gravity;
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
            if (!touching)
                touching = position.distance(target) < img.getWidth()/2;
            return touching;
        }
        bool isAlive() {
            return alive;
        }
    };
    
    enum GameState {
        GAME_STATE_TITLE = 0,
        GAME_STATE_GAME = 1,
        GAME_STATE_END = 2
    };
    
    class Game {
        
        void updateGame(const ofVec2f& heroPos){
            
            float dt = 1.f/60.f;
            gameTimer.update(dt);
            
            currentTime.set(gameTimer.getCurrentValue());
            
            if (!gameTimer.isAnimating() || helmets.empty()){
                endGame();
            } else {
                for (int i=0; i<helmets.size(); ++i){
                    shared_ptr<Helmet> h = helmets[i];
                    h->checkTargetIntersects(heroPos);
                    if (h.get()->isAlive()){
                        h.get()->update();
                    } else {
                        helmets.erase(helmets.begin() + i);
                    }
                }
            }
        };
        
        void drawTitleState(){
            ofDrawBitmapStringHighlight("Helmet Catchers", ofGetWidth()/2, ofGetHeight()/2);
        };
        void drawGameState(){
            for (auto h : helmets){
                h.get()->draw();
            }
        };
        void drawEndState(){
            ofDrawBitmapStringHighlight("You win", ofGetWidth()/2, ofGetHeight()/2);
        };
        
        vector<shared_ptr<Helmet>>  helmets;
        ofImage                     helmetImg;
        float                       startTime;
        GameState                   state;
        ofxAnimatableFloat          gameTimer;
        bool                        timerEnd;
        
    public:
        
        Game(){};
        ~Game(){
            helmets.clear();
        };
        
        void setup(){
            
            helmetImg.load("helmet.png");
            helmets.clear();
            state = GAME_STATE_TITLE;
            
            params.setName("Game");
            params.add(endTime.set("End Time", 2, 2, 25));
            params.add(currentTime.set("Curent Time", 0.f, 0.f, 25.f));
        };
        
        void update(const ofVec2f& heroPos){
            
            switch (state) {
                case GAME_STATE_TITLE:
                    break;
                case GAME_STATE_GAME: {
                    updateGame(heroPos);
                    break;
                }
                case GAME_STATE_END:
                    break;
                default:
                    break;
            }
        };
        
        void draw(){
            switch (state) {
                case GAME_STATE_TITLE:
                    drawTitleState();
                    break;
                case GAME_STATE_GAME:
                    drawGameState();
                    break;
                case GAME_STATE_END:
                    drawEndState();
                    break;
                default:
                    break;
            }
        };
        
        void startGame(const int& count){
            
            gameTimer.setDuration(endTime);
            gameTimer.setRepeatType(PLAY_ONCE);
            gameTimer.setRepeatTimes(0);
            gameTimer.setCurve(LINEAR);
            
            helmets.clear();
            for (int i=0; i<count; ++i){
                addRandomHelmet();
            }
            
            gameTimer.animateFromTo(0, endTime);
            state = GAME_STATE_GAME;
        };
        
        void endGame(){
            timerEnd = true;
            startTime = ofGetElapsedTimeMillis();
            state = GAME_STATE_END;
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
    };
    
}
