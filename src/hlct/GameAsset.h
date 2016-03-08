#pragma once

#include "ofMain.h"
#include "Constants.h"


namespace hlct {
    
    class ImageSwitcher {
        ofImage img0, img1;
        shared_ptr<ofImage> img;
    public:
        void setup(const string& path0, const string& path1){
            img0.load(path0);
            img1.load(path1);
            img = shared_ptr<ofImage>(new ofImage);
        };
        void draw(const GameState& state){
            if (state != GAME_STATE_GAME){
                int seconds = (int)(ofGetElapsedTimeMillis() / 2000);
                if (seconds % 2 == 0) {
                    *img = img0;
                } else {
                    *img = img1;
                }
                img->draw(0, 0);
            }
        };
    };
    
    class GameAsset {
        ImageSwitcher switchImgTitle, switchImgWin, switchImgLoose;
        
    public:
        void setup(){
            switchImgTitle.setup("game/title_0.png", "game/title_1.png");
            switchImgWin.setup("game/win_0.png", "game/win_1.png");
            switchImgLoose.setup("game/loose_0.png", "game/loose_1.png");
        }
        void draw(const GameState& state){
            switch (state) {
                case GAME_STATE_TITLE:
                    switchImgTitle.draw(state);
                    break;
                case GAME_STATE_END_WIN:
                    switchImgWin.draw(state);
                    break;
                case GAME_STATE_END_LOOSE:
                    switchImgLoose.draw(state);
                    break;
                default:
                    break;
            }
        }
    };
}