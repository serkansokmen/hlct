#pragma once

#include "ofMain.h"

namespace hlct {
    
    class LivesDisplay {
        
        ofRectangle drawRect;
        int         totalLives;
        
        ofImage     imgLive;
        ofImage     imgDead;
        
    public:
        void setup(const ofRectangle& rect,
                   const int& totalLives,
                   const string& liveImgPath,
                   const string& deadImgPath);
        
        void draw(const int& livesLeft);
    };
}