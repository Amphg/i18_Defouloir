#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	webcam.setup(320, 240);
	video = new unsigned char[webcam.getWidth()*webcam.getHeight() * 3];
	paf.allocate(webcam.getWidth(), webcam.getHeight(), GL_RGB);
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

	meshOriginal = meshWarped = ofMesh::sphere(250 ,20);
	meshWarped.setMode(OF_PRIMITIVE_TRIANGLES);
	
	
	
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
	int numOfVerts = meshOriginal.getNumVertices();
	
	float * audioData = new float[numOfVerts];
	fftLive.getFftPeakData(audioData, numOfVerts);

	float meshDisplacement = 100;

	for (int i = numOfVerts-1; i>=0; i--) {
		
		ofFloatColor sampleColor(webcam.getPixels()[i] / 255.f);		// b
	
		float audioValue = audioData[i];
		ofVec3f & vertOriginal = vertsOriginal[i];
		ofVec3f & vertWarped = vertsWarped[i];

		ofVec3f direction = vertOriginal.getNormalized();
		vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
		meshWarped.setVertex(i, vertWarped);
		//Pour afficher de l'info dans la console noire
		//cout << "Index : " << meshWarped.lenght << '\n';
		//cout << "Index : " << sampleColor << '\n';*/
		ofColor Color =  sampleColor;
	
		meshWarped.clearColors();
		//meshWarped.addIndex(numOfVerts);
	//	cout << "Index : " << meshWarped.getNumVertices() << '\n';
		//meshWarped.removeColor(numOfVerts);
		meshWarped.setColor(numOfVerts-1,sampleColor);
		
	}
	
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
	webcam.bind();
	meshWarped.drawWireframe();
	//meshWarped.drawFaces();
	webcam.unbind();
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