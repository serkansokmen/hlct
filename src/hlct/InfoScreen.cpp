#include "InfoScreen.h"


void hlct::InfoScreen::setup(const ofRectangle& stageRect,
                             const string& fontPath,
                             const int& fontSize,
                             const float& messageDuration,
                             const ofPixels& pixels, vector<string> messages){
    
    this->rect.set(stageRect);
    image.setFromPixels(pixels);
    
    for (auto msg : messages){
        ofxParagraph p = ofxParagraph(msg, rect.getWidth()/2);
        p.setFont(fontPath, fontSize);
        p.setWidth(rect.getWidth()/2);
        p.setAlignment(ofxParagraph::ALIGN_CENTER);
        p.setColor(ofColor::white);
        p.setSpacing(40);
        this->paragraphs.push_back(p);
    }
    
    timer.setup();
    timer.setDuration(messageDuration);
    timer.setRepeatType(LOOP_BACK_AND_FORTH);
    timer.setCurve(LINEAR);
    timer.animateTo(messages.size());
}


void hlct::InfoScreen::update(){
    timer.update(1.f/60.f);
    ofLogNotice(ofToString(timer.getCurrentValue()));
}


void hlct::InfoScreen::draw(){
    
    float imgW = image.getWidth();
    float imgH = image.getHeight();
    float rw = rect.getWidth();
    float rh = rect.getHeight();
    float imgX = (rw-imgW)/2;
    float imgY = (rh-imgH)/2 - 100;
    int idx = (int)timer.getCurrentValue();
    if (idx == paragraphs.size() && idx != 0) {
        idx--;
    }
    auto p = paragraphs[idx];
    float px = (rw - p.getWidth())/2;
    float py = imgY + imgH + 140;
    
    image.draw(imgX, imgY);
    p.draw(px, py);
}