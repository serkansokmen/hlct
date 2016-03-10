#pragma once

#include "ofMain.h"

namespace hlct {
    class ImagePack {
    public:
        
        ImagePack(){
            brand = shared_ptr<ofImage>(new ofImage);
            hero = shared_ptr<ofImage>(new ofImage);
            bait = shared_ptr<ofImage>(new ofImage);
            liveFull = shared_ptr<ofImage>(new ofImage);
            liveEmpty = shared_ptr<ofImage>(new ofImage);
            background = shared_ptr<ofImage>(new ofImage);
        }
        
        shared_ptr<ofImage> brand;
        shared_ptr<ofImage> hero;
        shared_ptr<ofImage> bait;
        shared_ptr<ofImage> liveFull;
        shared_ptr<ofImage> liveEmpty;
        shared_ptr<ofImage> background;
    };
}
