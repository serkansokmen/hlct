#include "Helmet.h"


void hlct::Helmet::setup(const ofPixels& helmetPixels, const int& sectionIndex){
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

void hlct::Helmet::update(const ofVec2f& heroPos){
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

void hlct::Helmet::draw(){
    img.draw(position, img.getWidth()*scale, img.getHeight()*scale);
}
