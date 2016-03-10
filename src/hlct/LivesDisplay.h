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
        void setup(const ImagePack& imgPack,
                   const int& totalLives);
        
        void draw(const ofRectangle& stageRect,
                  const int& livesLeft,
                  const float& scale);
    };
}