#pragma once

#include "ofMain.h"
#include "GameState.h"
#include "ImageSwitcher.h"
#include "Constants.h"


namespace hlct {
    
    class GameAsset {
        
        ImageSwitcher       title;
        ImageSwitcher       posing;
        ImageSwitcher       win;
        ImageSwitcher       loose;
        
        GameState           state;
        
    public:
        void setup(const ofRectangle& stageRect);
        void update(const GameState& state);
        void draw();
    };
}