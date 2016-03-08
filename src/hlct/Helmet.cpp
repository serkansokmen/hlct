#include "Helmet.h"


void hlct::Helmet::setup(const ofPixels& helmetPixels, const int& sectionIndex, const ofRectangle& rect){
    stageRect.set(rect);
    gravity = ofRandom(-5.f, -4.f);
    float sectionWidth = stageRect.getWidth() / HLCT_HELMET_SECTION_COUNT;
    float sectionX = ofRandom(0, sectionWidth);
    position.x = ofClamp(sectionX + sectionWidth * sectionIndex, HLCT_CLAMP_STAGE, stageRect.getWidth()-HLCT_CLAMP_STAGE);
    
    alive = true;
    img.setFromPixels(helmetPixels);
    win = false;
}

void hlct::Helmet::update(const ofRectangle& heroRect){
    if (alive && position.y <= stageRect.getHeight() - img.getHeight()) {
        intersectRect.set(position, img.getWidth(), img.getHeight());
        if (!win) {
            position.y -= gravity;
        } else {
            position.x = ofClamp(heroRect.getCenter().x - img.getWidth()*0.5,
                                 HLCT_CLAMP_STAGE,
                                 stageRect.getWidth()-HLCT_CLAMP_STAGE);
            position.y = heroRect.getTop() - img.getHeight()*1.5;
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
    img.draw(position);
    ofPopStyle();
}
