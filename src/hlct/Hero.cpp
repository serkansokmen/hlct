#include "Hero.h"


void hlct::Hero::setup(const ofVec2f& position, const ofPixels& heroPixels){
    
    this->position = position;
    img.setFromPixels(heroPixels);
}

void hlct::Hero::update(const ofRectangle& stageRect, const float& scale){
    this->stageRect.set(stageRect);
    this->scale = scale;
    this->intersectRect.set(position, img.getWidth()*scale, img.getHeight()*scale);
}

void hlct::Hero::draw(){
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::blue);
    ofDrawRectangle(intersectRect);
    ofSetColor(ofColor::white);
    ofFill();
    img.draw(intersectRect);
    ofPopStyle();
}
