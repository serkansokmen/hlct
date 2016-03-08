#pragma once

#include "ofMain.h"
#include "GameState.h"
#include "ImageSwitcher.h"
#include "Constants.h"


namespace hlct {
    
    class GameAsset {
        ImageSwitcher switchImgTitle, switchImgWin, switchImgLoose;
        
    public:
        void setup();
        void draw(const GameState& state);
    };
}