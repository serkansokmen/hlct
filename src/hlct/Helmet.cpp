#include "Helmet.h"


void hlct::Helmet::setup(const ofPixels& helmetPixels, const int& sectionIndex, const ofRectangle& rect){
    stageRect.set(rect);
    gravity = ofRandom(-5.f, -4.f);
    float sectionWidth = stageRect.getWidth() / HLCT_HELMET_SECTION_COUNT;
    float sectionX = ofRandom(0, sectionWidth);
    position.x = ofClamp(sectionX + sectionWidth * sectionIndex, HLCT_CLAMP_STAGE, stageRect.getWidth()-HLCT_CLAMP_STAGE);
    
    alive = true;
    img.setFromPixels(helmetPixels);
    scale = 0.5f;
    win = false;
}

void hlct::Helmet::update(const ofVec2f& heroPos){
    if (alive && position.y <= stageRect.getHeight() - img.getHeight()*scale) {
        intersectRect.set(position, img.getWidth()*scale*1.2, img.getHeight()*scale*1.2);
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
