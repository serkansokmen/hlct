#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "ofMain.h"
#include "ofApp.h"


int main(){
    int windowWidth = 1728;
    int windowHeight = 768;
    ofSetupOpenGL(windowWidth, windowHeight, OF_WINDOW);
    ofRunApp(new ofApp());
}
