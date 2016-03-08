#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxAnimatableFloat.h"

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
        
        ofxOscReceiver              receiver;
        ofImage                     receivedImage;
        
    public:
        
        ~Game();
        
        void setup();
        void update(const ofVec2f& pos);
        void draw();
        void startGame();
        void endGame();
        void addRandomHelmet();
        
        inline bool isRunning(){
            return state == GAME_STATE_GAME;
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
