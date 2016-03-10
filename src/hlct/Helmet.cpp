#include "Helmet.h"


void hlct::Helmet::setup(const ofRectangle& stageRect, const ofPixels& helmetPixels, const int& sectionIndex){
    
    this->stageRect.set(stageRect);
    
    img.setFromPixels(helmetPixels);
    
    gravity = ofRandom(-5.f, -4.f);
    float sectionWidth = stageRect.getWidth() / HLCT_HELMET_SECTION_COUNT;
    float sectionX = ofClamp(ofRandom(0, sectionWidth),
                             stageRect.getX() + img.getWidth() / 2,
                             stageRect.getX() + stageRect.getWidth() - img.getWidth());
    position.x = sectionX + sectionWidth * sectionIndex;
    
    alive = true;
    win = false;
}

void hlct::Helmet::update(const ofRectangle& stageRect, const ofRectangle& hitRect, const float& scale){
    
    this->stageRect.set(stageRect);
    this->scale = scale;
    
    float imgW = img.getWidth() * scale;
    float imgH = img.getHeight() * scale;
    
    if (alive && position.y <= stageRect.getBottom() - imgH/2) {
        if (!win) {
            position.y -= gravity * 1.2;
        } else {
            position.x = hitRect.getCenter().x - imgW * 0.5;
            position.y = hitRect.getTop() - imgH * 1.5;
        }
    } else {
        alive = false;
    }
    
    intersectRect.set(position, imgW, imgH);
}

void hlct::Helmet::draw(const bool& debug){
    img.draw(intersectRect);
    if (debug){
        ofPushStyle();
        ofNoFill();
        ofSetColor(ofColor::blue);
        ofDrawRectangle(intersectRect);
        ofPopStyle();
    }
}
