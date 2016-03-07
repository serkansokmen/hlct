#pragma once

#include "ofMain.h"
#include "Constants.h"


namespace hlct {
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
}