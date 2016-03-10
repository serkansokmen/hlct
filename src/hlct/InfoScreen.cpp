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
        ofxParagraph p = ofxParagraph(msg, stageRect.getWidth()/2);
        ofxTextBlock t;
        p.setWidth(stageRect.getWidth()/2);
        shared_ptr<ofxSmartFont> font = ofxSmartFont::get(HLCT_INFO_SCREEN_FONT_NAME);
        t.init(HLCT_INFO_SCREEN_FONT_PATH, HLCT_INFO_SCREEN_FONT_SIZE);
        t.setText(msg);
        t.setColor(255, 255, 255, 255);
        t.wrapTextX(stageRect.getWidth()/2);
        if (font){
            p.setFont(font);
        }
        p.setAlignment(ofxParagraph::ALIGN_CENTER);
        p.setColor(ofColor::white);
        p.setSpacing(40);
        p.setBorderPadding(40);
        this->paragraphs.push_back(p);
        this->texts.push_back(t);
    }
    
    timer->setDuration(messageDuration);
    timer->animateTo(messages.size());
}


void hlct::InfoScreen::update(){

    timer->update(HLCT_ANIM_UPDATE_CYCLE);
    
    int idx = (int)timer->getCurrentValue();
    if (idx == paragraphs.size() && idx != 0) {
        idx--;
    }
    this->msgIndex = idx;
    
    auto p = paragraphs[msgIndex];
    
    float imgW = image.getWidth();
    float imgH = image.getHeight();
    float rw = stageRect.getWidth();
    float rh = stageRect.getHeight();
    float px = (rw - p.getWidth())/2;
    float py = rectImage.getTop() + rectImage.getHeight() + 140;
    
    this->rectImage.set((rw-imgW)/2,
                        (rh-imgH)/2 - 100,
                        imgW, imgH);
    this->rectParagraph.set(px, py, p.getWidth(), p.getHeight());
    this->drawRect.set(px, rectImage.getTop(), p.getWidth(), imgH + rectImage.getHeight());
}


void hlct::InfoScreen::draw(){
    if (this->msgIndex != -1){
        auto p = paragraphs[msgIndex];
        image.draw(rectImage);
        p.draw(rectParagraph.getLeft(), rectParagraph.getTop());
        
        auto t = texts[msgIndex];
        t.drawCenter(rectParagraph.getCenter().x, rectImage.getBottom());
    }
}
