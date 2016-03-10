#pragma once

#include "ofMain.h"
#include "Constants.h"

namespace hlct {
    
    class Helmet {
        ofImage img;
        ofVec2f position;
        float   gravity;
        float   scale;
        bool    alive;
        bool    win;
        ofRectangle intersectRect;
        ofRectangle stageRect;
        
    public:
        void setup(const ofRectangle& stageRect, const ofPixels& helmetPixels, const int& sectionIndex);
        void update(const ofRectangle& stageRect, const ofRectangle& heroRect, const float& scale);
        void draw(const bool& debug);
        
        inline const ofVec2f& getPosition() {
            return position;
        }
        inline float getWidth() {
            return img.getWidth();
        }
        inline float getHeight() {
            return img.getHeight();
        }
        inline void setScale(const float& scale){
            this->scale = scale;
        }
        inline bool isAlive() {
            return alive;
        }
        inline bool isWin(const ofRectangle& rect) {
            if (!win) {
                win = intersectRect.intersects(rect);
            }
            return win;
        }
    };
}