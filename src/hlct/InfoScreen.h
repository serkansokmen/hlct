#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxSmartFont.h"
#include "ofxTextSuite.h"
#include "ofxAnimatableFloat.h"


namespace hlct {
    
    class InfoScreen {
        
        ofImage             image;
        ofRectangle         rectImage;
        ofRectangle         rectParagraph;
        
        ofRectangle         stageRect;
        
        shared_ptr<ofxAnimatableFloat>  timer;
        vector<ofxTextBlock>            texts;
        int                             msgIndex;
        
    public:
        
        InfoScreen();
        
        void setup(const ofRectangle& stageRect,
                   const float& messageDuration,
                   const ofPixels& pixels, vector<string> messages);
        void update(const ofRectangle& stageRect);
        void draw();
    };
}