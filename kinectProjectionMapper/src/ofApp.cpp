#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    // enable depth->video image calibration
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    nearThreshold = 255;
    farThreshold = 180;
    
    ofSetFrameRate(60);
    // Gui
    gui.setup();
    gui.setPosition(ofPoint(10,10));
    gui.add(polylineCount.setup("polyline Count", 20, 5,200));
    gui.add(strokeWidth.setup("stroke width", 1, 1,60));
    gui.add(fillSwitch.setup("Fill Switch", false));
    //    set server name
    syphonServer.setName("Kinect");
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        // update the cv images
        grayImage.flagImageChanged();
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 1, true);

        if (!contourFinder.blobs.empty())
        {
            ofxCvBlob& blob = contourFinder.blobs[0];
            
            // simplify the the blob polyline ...
            shape = ofPolyline(blob.pts).getResampledByCount(polylineCount);
            
            std::vector<ofPoint>& vertices = shape.getVertices();
            
            for (std::size_t i = 0; i < vertices.size(); ++i)
            {
                vertices[i] *= ofPoint(ofGetWidth() / kinect.getWidth(),
                                       ofGetHeight() / kinect.getHeight(), 1);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
 
    ofPath path;
    path.setFilled(true);
    path.setFillColor(ofColor(color));
    path.setStrokeColor(0);
    path.setStrokeWidth(strokeWidth);
    
    if(!fillSwitch){
        
        ofBackground(color);
        path.setFillColor(0);
        path.setStrokeColor(color);
        
        
    }

    for (ofPoint point : shape.getVertices())
    {
        path.lineTo(point);
        
       
    }
    
    path.close();
    path.draw();
    
    color = ofColor( 0, 255, 0 );
    
    syphonServer.publishScreen();
    
    if(showGui) {
        
        ofPushStyle();
        ofSetColor(255,255,255,255);
        gui.draw();
        ofPopStyle();
    }
    ofSetColor(255, 255, 255);
}
//------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            break;
        case '<':
        case ',':
            farThreshold --;
            if (farThreshold < 0) farThreshold = 0;
            break;
        case '+':
        case '=':
            nearThreshold ++;
            if (nearThreshold > 255) nearThreshold = 255;
            break;
        case '-':
            nearThreshold --;
            if (nearThreshold < 0) nearThreshold = 0;
            break;
    }
    cout << "Far: " << farThreshold << " Near: " << nearThreshold << std::endl;

    if(key == 'g') {
        showGui = !showGui;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}
