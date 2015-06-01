

#include "ofApp.h"
#include <iostream>
#include <vector>

int cameraDevice;
int inputAdioDevice;
using namespace ofxCv;
using namespace cv;

vector<string> imageMemory;
vector<int> colorMemory;
vector<int> colorMemory2;

string animals[6]={"image1.png","image2.png","image3.png","image4.png","image5.png","sunglasses.png"};

void ofApp::setup(){

    // Init camera, see the available video devices in the consolse
    drawPictures    = false;
    showVideo       = false;
    cameraDevice    = 0;
    
    //Camera settings
    camWidth 		= 640;
    camHeight 		= 480;
    camFrameRate    = 220;
    
    //Audio device, see the available audio devices in the consolse
    inputAdioDevice = 3;
    
    
    //finder.setup("haarcascade_frontalface_alt2.xml");
    finder.setup("haarcascade_frontalface_default.xml");
    
    //Face tracking presets, the more sensitive, the more processing power it takes choose between: Fast, Accurate , Sensitive
    finder.setPreset(ObjectFinder::Fast);

    
    reactionDistance = 10; //Tweak the osc output to fit trigger reaction for playing
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(6);
    
    if (showVideo) {
        ofSetBackgroundAuto(false);
        //ofEnableAlphaBlending();
        ofSetFrameRate(90);
    }else{
        ofBackground(255, 255, 255);
        ofEnableAlphaBlending();
        ofSetFrameRate(90);
        ofSetBackgroundAuto(true);
    }
    
    //List sounddevices in the
    soundStream.listDevices();
    
    //if you want to set a different device id
    soundStream.setDeviceID(inputAdioDevice);
    int bufferSize = 512;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    
    
    //Normal camera

    vidGrabber.setDeviceID(cameraDevice);
    vidGrabber.setDesiredFrameRate(camFrameRate);
    vidGrabber.initGrabber(camWidth,camHeight);


    imageMemory.assign(300,"NULL");
    colorMemory.assign(300,0);
    colorMemory2.assign(300,0);
    
    finder.setUseHistogramEqualization(true);
    finder.getTracker().setSmoothingRate(.5);
    
    // Change tracker persistence
    // "persistence" determines how many frames an object can last without being
    // seen until the tracker forgets about it. "maximumDistance" determines how
    // far an object can move until the tracker considers it a new object.

    finder.getTracker().setPersistence(150); // Default: 15
    finder.getTracker().setMaximumDistance(camWidth/2); // Default: 64

    
    // Init OSC sender
    sender.setup(HOST, PORT);
    
}

//--------------------------------------------------------------

void ofApp::update(){
    
    //Particle
    
    //scales the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //records the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record it drops the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    
    //particle end
    
    // Update camera and finder
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew())
    {
        videoTexture.setFromPixels(vidGrabber.getPixelsRef());
        //videoTexture.loadData(vidGrabber.getPixelsRef());
        
        videoTexture.mirror(false, true);
        videoTexture.update();
        
        finder.update(videoTexture);
    }
    
    // Resseting osc messages
    ofxOscMessage area1;
    ofxOscMessage area2;
    ofxOscMessage area3;
    ofxOscMessage area4;
    ofxOscMessage area5;
    ofxOscMessage area6;
    ofxOscMessage area7;
    
    //The messages read by OSCulator for each face.
    area1.setAddress("/face/area0/");
    area1.addIntArg(0);
    sender.sendMessage(area1);
    
    area2.setAddress("/face/area1/");
    area2.addIntArg(0);
    sender.sendMessage(area2);
    
    area3.setAddress("/face/area2/");
    area3.addIntArg(0);
    sender.sendMessage(area3);
    
    area4.setAddress("/face/area3/");
    area4.addIntArg(0);
    sender.sendMessage(area4);
    
    area5.setAddress("/face/area4/");
    area5.addIntArg(0);
    sender.sendMessage(area5);
    
    area6.setAddress("/face/area5/");
    area6.addIntArg(0);
    sender.sendMessage(area6);
    
    area7.setAddress("/face/area6/");
    area7.addIntArg(0);
    sender.sendMessage(area7);
    
    
 
    
    int labels=finder.getTracker().getCurrentLabels().size();
 
    
    for (int i = 0; i < labels; i++){
        
        int numlabel=finder.getLabel(i);
        int areaSize=finder.getObject(i).getArea();
        
        char str[50];
        sprintf(str, "/face/area%d/", i);
        
        ofxOscMessage area;
        
        area.setAddress(str);
        area.addIntArg(areaSize/reactionDistance);
        
        sender.sendMessage(area);


    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //Draw video
    if (showVideo) {
        
        float videoW=videoTexture.getWidth();
        float ratio=ofGetWidth()/videoW;
        ofScale(ratio, ratio);
        videoTexture.draw(0,0);
        
    }else{
   
        float videoW=videoTexture.getWidth();
        float ratio=ofGetWidth()/videoW;
        ofScale(ratio, ratio);
            
        ofSetColor(255, 255, 255,15);
        ofFill();
        ofRect(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
        
    }
    
    std::vector<unsigned int> trackerLables;
    
    trackerLables = finder.getTracker().getCurrentLabels();
    
    
    for(int i = 0; i < finder.size(); i++) {

        ofRectangle object = finder.getObjectSmoothed(i);
        
        reMappedX = object.x;
        reMappedY = object.y;
        
        
        
        //Shader
        float targetX = reMappedX;
        float dx = targetX - x;
        if(abs(dx) > 1) {
            x += dx * easing;
        }
        
        float targetY = reMappedY;
        float dy = targetY - y;
        if(abs(dy) > 1) {
            y += dy * easing;
        }
        
        if(!drawPictures){ //I removed the draw pictures code because I was not using it
        //DRAW PARTICLES
        //Check if images is in memory, and if it is load it
        if ((colorMemory.at(finder.getTracker().getLabelFromIndex(i)) != 0)){
        
            //Particle start
            if(!showVideo){
                ofSetColor(colorMemory[finder.getTracker().getLabelFromIndex(i)], scaledVol *100, colorMemory2[finder.getTracker().getLabelFromIndex(i)]);
                ofFill();
                
                ofCircle(reMappedX+object.width / 2.,reMappedY+ object.height * .42, 50+scaledVol * 200.0f);

                ofSetColor(35, 100, scaledVol * 135,50);
                ofFill();
                ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }else{
                ofSetColor(35, 100, scaledVol * 135,50);
                ofFill();
            ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }
            
            //Particle end

            
            //Adds it to memory
        }else{
            
            colorMemory[finder.getTracker().getLabelFromIndex(i)]=(int)ofRandom(1,255);
            colorMemory2[finder.getTracker().getLabelFromIndex(i)]=(int)ofRandom(1,255);
            
            //Particle start
            if(!showVideo){
                ofSetColor(colorMemory[finder.getTracker().getLabelFromIndex(i)], scaledVol *100, colorMemory2[finder.getTracker().getLabelFromIndex(i)]);
                ofFill();
                
                ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 50+scaledVol * 200.0f);
                
                ofSetColor(35, 100, scaledVol *135,50);
                ofFill();
                ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }else{
                ofSetColor(35, 100, scaledVol *135,50);
                ofFill();
            ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }
  
            
            //Particle end
            
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    int numCounted = 0;
    
    // goes through each sample and calculates the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //gets the mean of rms
    curVol /= (float)numCounted;
    
    //gets the root of rms
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}