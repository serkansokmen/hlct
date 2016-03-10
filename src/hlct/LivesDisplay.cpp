#include "LivesDisplay.h"


void hlct::LivesDisplay::setup(const hlct::ImagePack& imgPack,
                               const int& totalLives){
    
    this->totalLives = totalLives;
    
    full.setFromPixels(imgPack.liveFull->getPixels());
    dead.setFromPixels(imgPack.liveEmpty->getPixels());
}


void hlct::LivesDisplay::draw(const ofRectangle& stageRect, const int& livesLeft, const float& scale){
    
    float iconW = full.getWidth() * scale;
    float iconH = full.getHeight() * scale;
    float rectW = stageRect.getWidth();
    float rectH = stageRect.getHeight();
    float rectX = rectW - iconW * totalLives;
    float rectY = 0;
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(stageRect.getTopRight());
    ofTranslate(-iconW * this->totalLives, -iconH / 2);
    for (int i = 0; i < this->totalLives; i++){
        ofPushMatrix();
        ofTranslate(iconW * i, iconH/2);
        if (i < livesLeft) {
            ofSetColor(ofColor::white, 200);
            full.draw(0, 0, iconW, iconH);
        } else {
            ofSetColor(ofColor::white, 200);
            dead.draw(0, 0, iconW, iconH);
        }
        ofPopMatrix();
    }
    ofPopStyle();
    ofPopMatrix();
}