#include "ImageSwitcher.h"


void hlct::ImageSwitcher::setup(const string& path0, const string& path1){
    img0.load(path0);
    img1.load(path1);
    img = shared_ptr<ofImage>(new ofImage);
    
    timer.setup();
    timer.setDuration(1);
    timer.setRepeatType(LOOP_BACK_AND_FORTH);
    timer.setCurve(LINEAR);
    timer.animateTo(1.f);

    drawFirst = true;
}

void hlct::ImageSwitcher::update(const hlct::GameState& state){
    
    timer.update(1.f/60.f);
    
    if (state != GAME_STATE_GAME) {

        int val = (int)(timer.getCurrentValue()*10);
        drawFirst = val <= 5;
    }
}

void hlct::ImageSwitcher::draw(){
    if (drawFirst){
        *img = img0;
    } else {
        *img = img1;
    }
    img->draw(0, 0);
}
