#pragma once

#include "ofMain.h"
#include "ImagePack.h"


namespace hlct {
    
    class LivesDisplay {
        
        ofRectangle drawRect;
        int         totalLives;
        
        ofImage     full;
        ofImage     dead;
        
    public:
        void setup(const ofRectangle& stageRect,
                   const ImagePack& imgPack,
                   const int& totalLives);
        
        void draw(const int& livesLeft);
    };
}