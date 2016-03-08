#pragma once

#include "ofMain.h"
#include "GameState.h"


namespace hlct {

    class ImageSwitcher {
    
        ofImage img0, img1;
        shared_ptr<ofImage> img;

    public:
        void setup(const string& path0,
                   const string& path1);
        void update();
        void draw(const GameState& state);
    };
}
