#include "ImageSwitcher.h"


void hlct::ImageSwitcher::setup(const string& path0, const string& path1){
    img0.load(path0);
    img1.load(path1);
    img = shared_ptr<ofImage>(new ofImage);
};


void hlct::ImageSwitcher::draw(const hlct::GameState& state){
    if (state != GAME_STATE_GAME){
        int seconds = (int)(ofGetElapsedTimeMillis() / 2000);
        if (seconds % 2 == 0) {
            *img = img0;
        } else {
            *img = img1;
        }
        img->draw(0, 0);
    }
};