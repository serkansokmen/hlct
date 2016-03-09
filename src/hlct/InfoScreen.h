#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxParagraph.h"
#include "ofxAnimatableFloat.h"


namespace hlct {
    
    class InfoScreen {
        
        ofImage             image;
        ofRectangle         rectImage;
        ofRectangle         rectParagraph;
        
        ofRectangle         stageRect;
        ofRectangle         drawRect;
        
        shared_ptr<ofxAnimatableFloat>  timer;
        vector<ofxParagraph>            paragraphs;
        int                             paragraphIndex;
        
    public:
        
        InfoScreen();
        
        void setup(const ofRectangle& stageRect,
                   const string& fontPath,
                   const int& fontSize,
                   const float& messageDuration,
                   const ofPixels& pixels, vector<string> messages);
        void update();
        void draw();
        
        float getHeight(){
            return drawRect.getHeight();
        }
    };
}