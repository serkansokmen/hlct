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
    float rectX = stageRect.getWidth() - iconW * totalLives;
    float rectY = 30;
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(rectX, rectY);
    for (int i = 0; i < this->totalLives; i++){
        ofPushMatrix();
        ofTranslate(iconW * i, 0);
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