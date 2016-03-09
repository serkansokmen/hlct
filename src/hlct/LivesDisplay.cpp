#include "LivesDisplay.h"


void hlct::LivesDisplay::setup(const ofRectangle& stageRect,
                               const hlct::ImagePack& imgPack,
                               const int& totalLives){
    
    float x = stageRect.getWidth() - imgPack.liveFull->getWidth()*totalLives*0.5 - 70;
    float y = 30;
    float w = imgPack.liveFull->getWidth()*0.5;
    float h = imgPack.liveFull->getHeight()*0.5;
    
    this->drawRect.set(ofRectangle(x, y, w, h));
    this->totalLives = totalLives;
    
    full.setFromPixels(imgPack.liveFull->getPixels());
    dead.setFromPixels(imgPack.liveEmpty->getPixels());
}


void hlct::LivesDisplay::draw(const int& livesLeft){
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
            full.draw(0, 0, w, h);
        } else {
            ofSetColor(ofColor::white, 200);
            dead.draw(0, 0, w, h);
        }
        ofPopMatrix();
    }
    ofPopStyle();
    ofPopMatrix();
}