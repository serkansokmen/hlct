#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxSmartFont.h"
#include "ofxParagraph.h"
#include "ofxTextSuite.h"
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
        vector<ofxTextBlock>            texts;
        int                             msgIndex;
        
    public:
        
        InfoScreen();
        
        void setup(const ofRectangle& stageRect,
                   const float& messageDuration,
                   const ofPixels& pixels, vector<string> messages);
        void update();
        void draw();
        
        const ofRectangle& getContainerRectangle(){
            return drawRect;
        }
    };
}