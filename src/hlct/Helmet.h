#pragma once

#include "ofMain.h"
#include "Constants.h"

namespace hlct {
    
    class Helmet {
        ofImage img;
        ofVec2f position;
        float   gravity;
        bool    alive;
        bool    win;
        ofRectangle intersectRect;
        ofRectangle stageRect;
        
    public:
        void setup(const ofPixels& helmetPixels, const int& sectionIndex, const ofRectangle& stageRect);
        void update(const ofRectangle& heroRect);
        void draw();
        
        inline const ofVec2f& getPosition() {
            return position;
        }
        inline float getWidth() {
            return img.getWidth();
        }
        inline float getHeight() {
            return img.getHeight();
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