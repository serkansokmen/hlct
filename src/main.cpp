#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "ofMain.h"
#include "ofApp.h"


int main(){
    int windowWidth = 1440;
    int windowHeight = 900;
    ofSetupOpenGL(windowWidth, windowHeight, OF_WINDOW);
    ofRunApp(new ofApp());
}
