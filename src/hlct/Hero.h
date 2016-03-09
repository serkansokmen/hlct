#pragma once

#include "ofMain.h"
#include "Constants.h"

namespace hlct {
    
    class Hero {
        ofImage img;
        ofVec2f position;
        float   scale;
        
        ofRectangle intersectRect;
        ofRectangle headRect;
        ofRectangle stageRect;
        
    public:
        void setup(const ofVec2f& position, const ofPixels& heroPixels);
        void update(const ofRectangle& stageRect, const float& scale);
        void draw();
        
        inline const ofVec2f& moveTo(const ofVec2f& position) {
            this->position.set(ofClamp(position.x, HLCT_CLAMP_STAGE, stageRect.getWidth() - HLCT_CLAMP_STAGE),
                               position.y);
        }
        inline const ofVec2f& getPosition() {
            return this->position;
        }
        inline const ofRectangle& getHitRectangle() {
            return this->headRect;
        }
    };
}