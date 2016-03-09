#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"

#include "Hero.h"
#include "Helmet.h"
#include "GameState.h"
#include "InfoScreen.h"
#include "LivesDisplay.h"
#include "Constants.h"


namespace hlct {    

    class Game {
        
        inline void handleUserExists(bool& exists){
            if (!exists){
                if (state == GAME_STATE_GAME){
                    endGame();
                }
                bUserPosing = false;
            }
        };
        
        inline void handlePosing(bool& isPosing){
            if (state == GAME_STATE_TITLE || state == GAME_STATE_POSING){
                if (bUserExists) {
                    if (isPosing){
                        state = GAME_STATE_POSING;
                        if (!gameStartTimer.isAnimating()) {
                            gameStartTimer.animateFromTo(0.f, 1.f);
                        }
                    } else {
                        state = GAME_STATE_TITLE;
                        gameStartTimer.reset();
                    }
                } else {
                    state = GAME_STATE_TITLE;
                    gameStartTimer.reset();
                }
            }
        };
        
        inline void handleGameStart(bool& val){
            if (val) {
                startGame();
                bStart = false;
            }
        };
        
        inline void handleAddHelmet(bool& val){
            if (val && state == GAME_STATE_GAME) {
                addRandomHelmet();
            }
            bAddHelmet = false;
        };
        
        inline void resizeLoadingBar(const string& infoScreenName){
            loadingBarRect.setFromCenter(screens[infoScreenName].getContainerRectangle().getCenter(),
                                         400, 25);
            loadingBarRect.setY(screens[infoScreenName].getContainerRectangle().getBottom() + 120);
        };
        
        void setupInfoScreens();
        void drawLoadingBar(const ofRectangle& rect, const float& width);
        
        std::map<string, InfoScreen> screens;
        
        hlct::Hero                  hero;
        hlct::GameState             state;
        hlct::LivesDisplay          livesDisplay;
        hlct::ImagePack             imgPack;
        
        ofxAnimatableFloat          gameStartTimer, gameTimer, gameEndTimer;
        ofRectangle                 stageRect, loadingBarRect;
        
        ofxOscReceiver              receiver;
        
        float                       startTime;
        int                         livesLeft;
        bool                        timerEnd;
        
        vector<shared_ptr<hlct::Helmet > >  helmets;
        vector<shared_ptr<hlct::Helmet > >  winHelmets;
        
    public:
        
        Game();
        ~Game();
        
        void setup(const ofRectangle& stageRect);
        void update();
        void draw();
        void startGame();
        void endGame();
        void addRandomHelmet();
        
        inline void mouseMoved(int x, int y){
            if (!useOsc) {
//                hero.moveTo(ofVec2f(x, hero.getPosition().y));
                hero.moveTo(ofVec2f(x, y));
            }
        };
        inline void resize(const ofRectangle& stageRect){
            this->stageRect.set(stageRect);
        };
        inline bool isRunning(){
            return state == GAME_STATE_GAME;
        };
        
        ofParameterGroup    params;
        ofParameter<string> currentTimeStr;
        ofParameter<float>  scaleHero;
        ofParameter<float>  scaleBait;
        ofParameter<float>  scaleBaitWin;
        ofParameter<float>  offsetBaitWin;
        ofParameter<float>  scaleLive;
        ofParameter<float>  currentTime;
        ofParameter<int>    endTime;
        ofParameter<int>    score;
        ofParameter<int>    helmetSection;
        ofParameter<bool>   useOsc;
        ofParameter<bool>   bUserExists;
        ofParameter<bool>   bUserPosing;
        ofParameter<bool>   bStart;
        ofParameter<bool>   bAddHelmet;
        ofParameter<bool>   bPaused;
    };
}
