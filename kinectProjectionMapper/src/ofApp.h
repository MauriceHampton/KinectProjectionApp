#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    
    ofxKinect kinect;
    //defining openCv stuff
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    ofxCvContourFinder contourFinder;
    
    ofPolyline shape;
    
    int nearThreshold;
    int farThreshold;
    ofColor color;
    // gui
    bool showGui;
    ofxPanel gui;
    ofxSlider<int> polylineCount;
    ofxSlider<int> strokeWidth;
    ofxToggle fillSwitch;
    
    ofxSyphonServer syphonServer;
};
