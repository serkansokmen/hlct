#pragma once

#include "ofMain.h"


namespace hlct {
    class StageBackground {
        
        ofImage     img;
        ofVboMesh   mesh;
        float       imgW;
        float       imgH;
        
    public:
        void setup(const ofRectangle& stageRect, const ofPixels& pixels){
            img.setFromPixels(pixels);
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            this->setStageRectangle(stageRect);
        };
        
        void setStageRectangle(const ofRectangle& stageRect){
            
            mesh.clear();
            
            float sx = stageRect.getX();
            float sy = stageRect.getY();
            float sw = stageRect.getWidth();
            float sh = stageRect.getHeight();
            
            imgW = img.getWidth();
            imgH = sh * imgW / sw;
            
            mesh.addVertex(stageRect.getTopLeft());
            mesh.addTexCoord(ofVec2f(0, 0));
            mesh.addColor(ofColor::white);
            
            mesh.addVertex(stageRect.getBottomLeft());
            mesh.addTexCoord(ofVec2f(0, imgH));
            mesh.addColor(ofColor::white);
            
            mesh.addVertex(stageRect.getBottomRight());
            mesh.addTexCoord(ofVec2f(imgW, imgH));
            mesh.addColor(ofColor::white);
            
            mesh.addVertex(stageRect.getTopRight());
            mesh.addTexCoord(ofVec2f(imgW, 0));
            mesh.addColor(ofColor::white);
        };
        
        void draw(const ofRectangle& stageRect){
            img.getTexture().bind();
            mesh.draw();
            img.getTexture().unbind();
        };
    };
}