#pragma once

#include "ofMain.h"

namespace hlct {
    class LivesDisplay {
        
        ofRectangle drawRect;
        int         totalLives;
        
        ofImage     imgLive;
        ofImage     imgDead;
        
    public:
        void setup(const ofRectangle& rect, const int& totalLives, const string& liveImgPath, const string& deadImgPath){

            this->drawRect.set(rect);
            this->totalLives = totalLives;
            
            imgLive.load(liveImgPath);
            imgDead.load(deadImgPath);
        };
        
        void draw(const int& livesLeft){
            float w = this->drawRect.getWidth();
            float h = this->drawRect.getHeight();
            
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(this->drawRect.getTopLeft());
            for (int i=0; i<this->totalLives; i++){
                ofPushMatrix();
                ofTranslate(w*i, 0);
                if (i < livesLeft) {
                    ofSetColor(ofColor::white, 200);
                    imgLive.draw(0, 0, w, h);
                } else {
                    ofSetColor(ofColor::white, 200);
                    imgDead.draw(0, 0, w, h);
                }
                ofPopMatrix();
            }
            ofPopStyle();
            ofPopMatrix();
        };
    };
}