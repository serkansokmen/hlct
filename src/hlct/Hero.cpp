#include "Hero.h"


void hlct::Hero::setup(const ofVec2f& position, const ofPixels& heroPixels){
    
    this->position = position;
    img.setFromPixels(heroPixels);
}

void hlct::Hero::update(const ofRectangle& stageRect, const float& scale){
    this->stageRect.set(stageRect);
    this->scale = scale;
    this->position.x = ofClamp(this->position.x,
                               stageRect.getX(),
                               stageRect.getX() + stageRect.getWidth() - img.getWidth() * scale);
    this->position.y = ofClamp(this->position.y,
                               stageRect.getY() + stageRect.getHeight() - img.getHeight() * scale,
                               stageRect.getY() + stageRect.getHeight() + img.getHeight() * scale);
    this->intersectRect.set(position, img.getWidth() * scale, img.getHeight() * scale);
    
    this->headRect.setFromCenter(position, intersectRect.getWidth() * 0.75, intersectRect.getWidth() * 0.75);
    this->headRect.setX((intersectRect.getWidth() - headRect.getWidth())/2 + intersectRect.getLeft());
    this->headRect.setY(intersectRect.getTop());
}

void hlct::Hero::draw(){
    img.draw(intersectRect);
}
