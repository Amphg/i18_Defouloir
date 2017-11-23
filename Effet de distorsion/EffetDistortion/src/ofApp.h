/**
*
* OFDevCon Example Code Sprint
*
* This example shows building a mesh, texturing it with a webcam, and extruding the vertices based on the pixel brightness
* Moving the mouse also rotates the mesh to see it at different angles
*
* Created by Tim Gfrerer and James George for openFrameworks workshop at Waves Festival Vienna sponsored by Lichterloh and Pratersauna
* Adapted during ofDevCon on 2/23/2012
*/

#pragma once

#include "ofMain.h"
#include "ofxFFTLive.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	ofxFFTLive fftLive;
	ofxPanel gui;
	ofxSlider<float> audioThreshold;
	ofxSlider<float> audioPeakDecay;
	ofxSlider<float> audioMaxDecay;
	ofxToggle audioMirror;
	ofCamera cam; // add mouse controls for camera movement
	float extrusionAmount;
	ofVboMesh mainMesh;
	ofVideoGrabber vidGrabber;
	ofImage Screenshot;
	ofMesh meshC;
	ofTexture videoTexture;
	ofTexture videoTexture1;
	ofTexture videoTexture2;
	unsigned char * output1;
	unsigned char * output2;
	unsigned char * output3;
	unsigned char * output4;
	
	int w;
	int h;
	float kernel[3][3];
	float kernel2[7][7];
	float wavelength = 20.50;

	float amplitude = 10;
	float noiseScale = 2;
	float distortion = 25;
	float iSpeed = 3.0;
	ofxPanel wuv;
	ofxFloatSlider wavelengthslider;
	ofxFloatSlider amplitudeslider;
	ofxFloatSlider noiseScaleslider;
	ofxFloatSlider distortionslider;
	ofxFloatSlider Speedslider;
	ofMesh meshOriginal;
	ofMesh meshWarped;
	unsigned char * video;
	ofImage image;
	ofPixels fboPixels;
	ofTexture paf;
	ofColor rouge;
};
