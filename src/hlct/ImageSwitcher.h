#pragma once

#include "ofMain.h"
#include "GameState.h"
#include "ofxAnimatableFloat.h"


namespace hlct {

    class ImageSwitcher {
    
        ofImage             img0, img1;
        shared_ptr<ofImage> img;
        ofxAnimatableFloat  timer;
        bool                drawFirst;

    public:
        void setup(const string& path0,
                   const string& path1);
        void update(const GameState& state);
        void draw();
    };
}
