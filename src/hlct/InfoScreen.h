#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxParagraph.h"
#include "ofxAnimatableFloat.h"


namespace hlct {
    
    class InfoScreen {
        
        ofImage             image;
        ofxAnimatableFloat  timer;
        ofRectangle         rect;
        
        vector<ofxParagraph> paragraphs;
        
    public:
        void setup(const ofRectangle& stageRect,
                   const string& fontPath,
                   const int& fontSize,
                   const float& messageDuration,
                   const ofPixels& pixels, vector<string> messages);
        void update();
        void draw();
    };
}