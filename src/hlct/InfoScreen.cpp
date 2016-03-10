#include "InfoScreen.h"


hlct::InfoScreen::InfoScreen(){
    timer = shared_ptr<ofxAnimatableFloat>(new ofxAnimatableFloat);
    timer->setup();
    timer->setRepeatType(LOOP_BACK_AND_FORTH);
    timer->setCurve(LINEAR);
    
    this->msgIndex = -1;
}


void hlct::InfoScreen::setup(const ofRectangle& stageRect,
                             const float& messageDuration,
                             const ofPixels& pixels, vector<string> messages){
    
    this->stageRect.set(stageRect);
    this->image.setFromPixels(pixels);
    
    for (auto msg : messages){
        ofxTextBlock t;
//        shared_ptr<ofxSmartFont> font = ofxSmartFont::get(HLCT_INFO_SCREEN_FONT_NAME);
        t.init(HLCT_INFO_SCREEN_FONT_PATH, HLCT_INFO_SCREEN_FONT_SIZE);
        t.setText(msg);
        t.setColor(255, 255, 255, 255);
        t.wrapTextX(stageRect.getWidth()/2);
        this->texts.push_back(t);
    }
    
    timer->setDuration(messageDuration);
    timer->animateTo(messages.size());
}


void hlct::InfoScreen::update(const ofRectangle& stageRect){
    this->stageRect.set(stageRect);
    timer->update(HLCT_ANIM_UPDATE_CYCLE);
    
    int idx = (int)timer->getCurrentValue();
    if (idx == texts.size() && idx != 0) {
        idx--;
    }
    this->msgIndex = idx;
    auto t = texts[msgIndex];
    
    float imgW = image.getWidth();
    float imgH = image.getHeight();
    float rw = stageRect.getWidth();
    float rh = stageRect.getHeight();
    float tx = (rw - t.getWidth())/2;
    float ty = rectImage.getTop() + rectImage.getHeight() + 140;
    
    this->rectImage.set(stageRect.getX() + (rw-imgW)/2,
                        stageRect.getY() + (rh-imgH)/2 - 100,
                        imgW, imgH);
    this->rectParagraph.set(stageRect.getX() + tx, stageRect.getY() + ty, t.getWidth(), t.getHeight());
//    this->drawRect.set(tx, rectImage.getTop(), t.getWidth(), imgH + rectImage.getHeight());
}


void hlct::InfoScreen::draw(){
    if (this->msgIndex != -1){
        image.draw(rectImage);
        auto t = texts[msgIndex];
        t.drawCenter(rectParagraph.getCenter().x, rectImage.getBottom());
    }
}
