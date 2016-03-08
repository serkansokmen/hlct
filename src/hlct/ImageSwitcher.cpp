#include "ImageSwitcher.h"


void hlct::ImageSwitcher::setup(const string& path0, const string& path1, const ofRectangle& rect){
    img0.load(path0);
    img1.load(path1);
    img = shared_ptr<ofImage>(new ofImage);
    
    timer.setup();
    timer.setDuration(2);
    timer.setRepeatType(LOOP_BACK_AND_FORTH);
    timer.setCurve(LINEAR);
    timer.animateTo(1.f);
    
    drawRect.set(rect);
}

void hlct::ImageSwitcher::update(const hlct::GameState& state){
    
    timer.update(1.f/60.f);
    
    if (state != GAME_STATE_GAME) {
        float val = (timer.getCurrentValue()*10);
        if (val <= 5.5){
            *img = img0;
        } else {
            *img = img1;
        }
    }
    drawRect.setFromCenter(drawRect.getCenter(), img->getWidth(), img->getHeight());
}

void hlct::ImageSwitcher::draw(){
    img->draw(drawRect);
}
