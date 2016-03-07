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
        void setup(const ofPixels& helmetPixels, const int& sectionIndex){
            
            gravity = ofRandom(-5.f, -4.f);
            float sectionWidth = ofGetWidth() / HLCT_HELMET_SECTION_COUNT;
            float sectionX = ofRandom(0, sectionWidth);
            position.x = ofClamp(sectionX + sectionWidth * sectionIndex, 100, ofGetWidth()-100);
            
            alive = true;
            img.setFromPixels(helmetPixels);
            //            scale = ofClamp(ofRandomuf(), 0.6f, 1.f);
            scale = 0.5f;
            win = false;
        }
        void update(const ofVec2f& heroPos){
            if (alive && position.y <= ofGetHeight() - img.getHeight()*scale) {
                intersectRect.set(position, img.getWidth()*scale, img.getHeight()*scale);
                if (!win) {
                    position.y -= gravity;
                } else {
                    position.x = heroPos.x - img.getWidth()*scale*0.5;
                    position.y = heroPos.y - img.getHeight()*scale*1.5;
                }
            } else {
                alive = false;
            }
            if (win) {
                scale = 0.5f;
            }
        }
        void draw(){
            img.draw(position, img.getWidth()*scale, img.getHeight()*scale);
        }
        
        const ofVec2f& getPosition() {
            return position;
        }
        float getWidth() {
            return img.getWidth()*scale;
        }
        float getHeight() {
            return img.getHeight()*scale;
        }
        bool isAlive() {
            return alive;
        }
        bool isWin(const ofVec2f& pos) {
            if (!win) {
                win = intersectRect.inside(pos);
            }
            return win;
        }
    };
}