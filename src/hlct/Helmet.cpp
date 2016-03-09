#include "Helmet.h"


void hlct::Helmet::setup(const ofRectangle& stageRect, const ofPixels& helmetPixels, const int& sectionIndex){
    
    this->stageRect.set(stageRect);
    
    img.setFromPixels(helmetPixels);
    
    gravity = ofRandom(-5.f, -4.f);
    float sectionWidth = stageRect.getWidth() / HLCT_HELMET_SECTION_COUNT;
    float sectionX = ofRandom(0, sectionWidth);
    position.x = sectionX + sectionWidth * sectionIndex;
    
    alive = true;
    win = false;
}

void hlct::Helmet::update(const ofRectangle& stageRect, const ofRectangle& heroRect, const float& scale){
    
    this->stageRect.set(stageRect);
    this->scale = scale;
    
    float imgW = img.getWidth() * scale;
    float imgH = img.getHeight() * scale;
    intersectRect.set(position, imgW, imgH);
    
    if (alive && position.y <= stageRect.getBottom()) {
        if (!win) {
            position.y -= gravity * 1.2;
        } else {
            position.x = heroRect.getCenter().x - img.getWidth() * scale * 0.5;
            position.y = heroRect.getTop() - img.getHeight() * scale * 1.5;
        }
    } else {
        alive = false;
    }
}

void hlct::Helmet::draw(){
    ofPushStyle();
//    ofNoFill();
//    ofSetColor(ofColor::blue);
//    ofDrawRectangle(intersectRect);
    ofSetColor(ofColor::white);
    ofFill();
    img.draw(intersectRect);
    ofPopStyle();
}
