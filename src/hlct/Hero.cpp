#include "Hero.h"


void hlct::Hero::setup(const ofVec2f& position, const ofPixels& heroPixels){
    img.setFromPixels(heroPixels);
    this->position.x = position.x + img.getWidth() / 2;
    this->position.y = position.y;
}

void hlct::Hero::update(const ofRectangle& stageRect, const float& scale){
    
    float sX = stageRect.getX();
    float sY = stageRect.getY();
    float sW = stageRect.getWidth();
    float sH = stageRect.getHeight();
    float heroX = position.x;
    float heroY = position.y;
    float heroW = img.getWidth() * scale;
    float heroH = img.getHeight() * scale;
    
    this->stageRect.set(stageRect);
    this->scale = scale;
    
    heroX = ofClamp(heroX, sX, sX + sW + heroW);
    heroY = sY + sH - heroH / 2;
    
    this->position.set(heroX, heroY);
    this->intersectRect.setFromCenter(position.x, position.y, heroW, heroH);
    
    this->headRect.setFromCenter(position, intersectRect.getWidth() * 0.75, intersectRect.getWidth() * 0.75);
    this->headRect.setX((intersectRect.getWidth() - headRect.getWidth())/2 + intersectRect.getLeft());
    this->headRect.setY(intersectRect.getTop());
}

void hlct::Hero::draw(const bool& debug){
    img.draw(intersectRect);
    if (debug){
        ofPushStyle();
        ofNoFill();
        ofSetColor(ofColor::blue);
        ofDrawRectangle(intersectRect);
        ofSetColor(ofColor::green);
        ofDrawRectangle(headRect);
        ofPopStyle();
    }
}
