#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"

#include "Helmet.h"
#include "GameState.h"
#include "GameAsset.h"
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
        void drawLoadingBar(const ofRectangle& rect, const float& width);
        
        ofVec2f                     heroPos;
        GameAsset                   gameAsset;
        GameState                   state;
        LivesDisplay                livesDisplay;
        
        ofxAnimatableFloat          gameStartTimer, gameTimer, gameEndTimer;
        ofRectangle                 stageRect, loadingBarRect;
        
        ofxOscReceiver              receiver;
        ImagePack                   imgPack;
        
        float                       startTime;
        int                         livesLeft;
        bool                        timerEnd;
        
        vector<shared_ptr<Helmet>>  helmets;
        vector<shared_ptr<Helmet>>  winHelmets;
        
    public:
        
        Game();
        ~Game();
        
        void setup(const ofRectangle& stageRect);
        void update();
        void draw();
        void startGame();
        void endGame();
        void addRandomHelmet();
        
        inline bool isRunning(){
            return state == GAME_STATE_GAME;
        };
        inline ofVec2f getHeroPosition(){
            return heroPos;
        };
        inline void mouseMoved(int x, int y){
            if (!useOsc) {
                heroPos.x = x;
            }
        };
        
        ofParameterGroup    params;
        ofParameter<string> currentTimeStr;
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
