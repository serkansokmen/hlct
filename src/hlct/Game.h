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
        
        ofxAnimatableOfPoint        heroPos;
        
        GameAsset                   gameAsset;
        GameState                   state;
        LivesDisplay                livesDisplay;
        
        ofxAnimatableFloat          gameTimer;
        
        ofxOscReceiver              receiver;
        
        ofImage                     helmetImg, helmetWhiteImg, helmetOutlineImg;
        ofImage                     receivedImage;
        float                       startTime;
        int                         livesLeft;
        bool                        timerEnd;
        
        vector<shared_ptr<Helmet>>  helmets;
        vector<shared_ptr<Helmet>>  winHelmets;
        
    public:
        
        ~Game();
        
        void setup();
        void update();
        void draw();
        void startGame();
        void endGame();
        void addRandomHelmet();
        
        inline bool isRunning(){
            return state == GAME_STATE_GAME;
        };
        inline ofVec2f getHeroPosition(){
            return heroPos.getCurrentPosition();
        };
        inline void mouseMoved(int x, int y){
            if (!useOsc) {
                heroPos.animateTo(ofVec2f(x, heroPos.getCurrentPosition().y));
            }
        };
        
        ofParameterGroup    params;
        ofParameter<string> currentTimeStr;
        ofParameter<float>  currentTime;
        ofParameter<int>    endTime;
        ofParameter<int>    score;
        ofParameter<int>    helmetSection;
        ofParameter<bool>   useOsc;
        ofParameter<bool>   bStart;
        ofParameter<bool>   bAddHelmet;
        ofParameter<bool>   bPaused;
    };
}
