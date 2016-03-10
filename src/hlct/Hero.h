#pragma once

#include "ofMain.h"
#include "Constants.h"

namespace hlct {
    
    class Hero {
        ofImage img;
        ofVec2f position;
        float   scale;
        float   offsetY;
        
        ofRectangle intersectRect;
        ofRectangle headRect;
        ofRectangle stageRect;
        
    public:
        void setup(const ofVec2f& position, const ofPixels& heroPixels);
        void update(const ofRectangle& stageRect, const float& scale);
        void draw(const bool& debug);
        
        inline void moveTo(const float& x) {
            this->position.x = ofClamp(x, stageRect.getX() + HLCT_CLAMP_STAGE, stageRect.getX() + stageRect.getWidth() - HLCT_CLAMP_STAGE);
        }
        inline void setScale(const float& scale){
            this->scale = scale;
        }
        inline const ofVec2f& getPosition() {
            return this->position;
        }
        inline const ofRectangle& getHitRectangle() {
            return this->headRect;
        }
    };
}