#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	webcam.setup(640, 480);
	video = new unsigned char[webcam.getWidth()*webcam.getHeight() * 3];

	ofBackground(50);
	ofSetFrameRate(30);

	fftLive.setMirrorData(false);
	fftLive.setup();

	string guiPath = "audio.xml";
	gui.setup("audio", guiPath, 20, 20);
	gui.add(audioThreshold.setup("audioThreshold", 1.0, 0.0, 1.0));
	gui.add(audioPeakDecay.setup("audioPeakDecay", 0.915, 0.0, 1.0));
	gui.add(audioMaxDecay.setup("audioMaxDecay", 0.995, 0.0, 1.0));
	gui.add(audioMirror.setup("audioMirror", true));
	gui.loadFromFile(guiPath);
	meshWarped.enableColors();

	meshOriginal = meshWarped = ofMesh::sphere(150, 320);
	
	//ofFloatColor sampleColor[860];

	
}

//--------------------------------------------------------------
void ofApp::update() {
	webcam.update();
	fftLive.setThreshold(audioThreshold);
	fftLive.setPeakDecay(audioPeakDecay);
	fftLive.setMaxDecay(audioMaxDecay);
	fftLive.setMirrorData(audioMirror);
	fftLive.update();

	//---------------------------------------------------------- dispacing mesh using audio.
	vector<ofVec3f> & vertsOriginal = meshOriginal.getVertices();
	vector<ofVec3f> & vertsWarped = meshWarped.getVertices();
	
	meshWarped.clearColors();
	int numOfVerts = meshWarped.getNumVertices();
	sampleColor.resize(numOfVerts);
	
	//meshWarped.addColors(sampleColor);
	
	float * audioData = new float[numOfVerts];
	fftLive.getFftPeakData(audioData, numOfVerts);

	float meshDisplacement = 200;
	ofVec3f & vertOriginal = vertsOriginal[0];
	ofVec3f & vertWarped = vertsWarped[0];
	meshWarped.addColors(sampleColor);
	//cout << "Couleur : " << numOfVerts << '\n';
	//cout << "Couleur : " << webcam.getHeight()*webcam.getWidth() << '\n';
	for (int i = 0; i<numOfVerts; i++) {
		
		//ofFloatColor sampleColor(webcam.getPixels()[i] / 255.f;	 webcam.getPixels()[i + 1] / 255.f;	 webcam.getPixels()[i + 2] / 255.f; );	// b
		//cout << "Couleur : " << numOfVerts << '\n';
	//	cout << "Couleur : " << sampleColor.size() << '\n';
		//sampleColor[i] = (255,255,255);
		
		float audioValue = audioData[i];
		vertOriginal = vertsOriginal[i];
		vertWarped = vertsWarped[i];
		sampleColor[i].r = webcam.getPixels()[i*3] / 255.f;
		sampleColor[i].g = webcam.getPixels()[(i*3  + 1)] / 255.f; 
		sampleColor[i].b = webcam.getPixels()[(i*3+2)] / 255.f;
		ofVec3f direction = vertOriginal.getNormalized();
		vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
		meshWarped.setVertex(i, vertWarped);
		//meshWarped.setColor(i, sampleColor[i]);
		//Pour afficher de l'info dans la console noire
		//cout << "Index : " << meshWarped.lenght << '\n';
	//	cout << "Index : " << sampleColor [i]<< '\n';
		//cout << "Index : " << << '\n';
		//meshWarped.addColors
		
		//meshWarped.clearColors();
		//meshWarped.addIndex(numOfVerts);
	//	meshWarped.removeIndex(861);
		//meshWarped.addColor(green);
		//meshWarped.addColors(sampleColor);
	//	cout << "Index : " << meshWarped.getNumVertices() << '\n';
		//meshWarped.removeColor(numOfVerts);
		//cout << "Couleur : " << meshWarped.getColor(0) << '\n';
	//	meshWarped.getColor(5);
		//meshWarped.setColor(numOfVerts-1,green);
		meshWarped.setColor(i, sampleColor[i]);
	
		
	}
	//cout << "Index : " << meshWarped.getColor(150) << '\n';
	//meshWarped.addColors(sampleColor);
		//cout << "Index : " << meshWarped.getColor(0) << '\n';
		//cout << "Index : " << meshWarped.getColor(150) << '\n';
	
	delete[] audioData;
	
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);

	int w = OFX_FFT_WIDTH;
	int h = OFX_FFT_HEIGHT;
	int x = 20;
	int y = ofGetHeight() - h - 20;
	
	//video = webcam.getPixels();
	//paf.loadData(video, x, y, GL_RGB);
	ofSetColor(255);
	webcam.draw(0, 0, 320, 240);
	cam.begin();
	//webcam.bind();
	meshWarped.drawFaces();
	//webcam.unbind();
	
	cam.end();
	//000000000gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}