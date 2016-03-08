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
        
    public:
        void setup(const ofPixels& helmetPixels, const int& sectionIndex);
        void update(const ofVec2f& heroPos);
        void draw();
        
        inline const ofVec2f& getPosition() {
            return position;
        }
        inline float getWidth() {
            return img.getWidth()*scale;
        }
        inline float getHeight() {
            return img.getHeight()*scale;
        }
        inline bool isAlive() {
            return alive;
        }
        inline bool isWin(const ofVec2f& pos) {
            if (!win) {
                win = intersectRect.inside(pos);
            }
            return win;
        }
    };
}