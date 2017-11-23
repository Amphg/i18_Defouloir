


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	video = new unsigned char[vidGrabber.getWidth()*vidGrabber.getHeight() * 3];
	paf.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), GL_RGB);
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

	meshOriginal = meshWarped = ofMesh::sphere(250, 30);
	wuv.setup();
	wuv.add(wavelengthslider.setup("longueur d'onde", 0, 0, 100));
	wuv.add(amplitudeslider.setup("amplitude", 0, 0, 100));
	wuv.add(noiseScaleslider.setup("bruit", 0, 0, 100));
	wuv.add(distortionslider.setup("distortion", 0, 0, 100));
	wuv.add(Speedslider.setup("vitesse", 0, 0, 100));
	w = 320;
	h = 240;
	vidGrabber.setup(w, h);
	vidGrabber.setVerbose(true);
	output1 = new unsigned char[w*h * 3];
	output2 = new unsigned char[w*h];
	output3 = new unsigned char[w*h];
	output4 = new unsigned char[w*h];

	videoTexture.allocate(w, h, GL_RGB);
	videoTexture1.allocate(w, h, GL_LUMINANCE);
	fftLive.setMirrorData(false);

	fftLive.setup();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(66, 66, 66);

	//initialize the video grabber
	


	//store the width and height for convenience
	int width = vidGrabber.getWidth();
	int height = vidGrabber.getHeight();

	//add one vertex to the mesh for each pixel
	for (int y = 0; y < height; y++) {
		for (int x = 0; x<width; x++) {
			mainMesh.addVertex(ofPoint(x, y, 0));	// mesh index = x + y*width
													// this replicates the pixel array within the camera bitmap...
			mainMesh.addColor(ofFloatColor(0, 0, 0));  // placeholder for colour data, we'll get this from the camera
		}
	}

	for (int y = 0; y<height - 1; y++) {
		for (int x = 0; x<width - 1; x++) {
			mainMesh.addIndex(x + y*width);				// 0
			mainMesh.addIndex((x + 1) + y*width);			// 1
			mainMesh.addIndex(x + (y + 1)*width);			// 10

			mainMesh.addIndex((x + 1) + y*width);			// 1
			mainMesh.addIndex((x + 1) + (y + 1)*width);		// 11
			mainMesh.addIndex(x + (y + 1)*width);			// 10
		}
	}

	//this is an annoying thing that is used to flip the camera
	cam.setScale(1, -1, 1);


	//this determines how much we push the meshes out
	extrusionAmount = 3;

	
	gui.setup("audio", guiPath, 20, 20);
	gui.add(audioThreshold.setup("audioThreshold", 1.0, 0.0, 1.0));
	gui.add(audioPeakDecay.setup("audioPeakDecay", 0.915, 0.0, 1.0));
	gui.add(audioMaxDecay.setup("audioMaxDecay", 0.995, 0.0, 1.0));
	gui.add(audioMirror.setup("audioMirror", true));
	

	meshC = ofMesh::sphere(200, 30);
	meshOriginal = meshWarped = ofMesh::sphere(250, 30);
}

//--------------------------------------------------------------
void ofApp::update() {
	fftLive.setThreshold(audioThreshold);
	fftLive.setPeakDecay(audioPeakDecay);
	fftLive.setMaxDecay(audioMaxDecay);
	fftLive.setMirrorData(audioMirror);
	fftLive.update();
	
	//grab a new frame
	vidGrabber.update();

	//update the mesh if we have a new frame
	if (vidGrabber.isFrameNew()) {
		if (ofGetElapsedTimef()<15) {

		float * AudioData = new float[vidGrabber.getWidth()*vidGrabber.getHeight()];
		
		fftLive.getFftPeakData(AudioData, vidGrabber.getWidth()*vidGrabber.getHeight());
		
		//this determines how far we extrude the mesh
		for (int i = 0; i<vidGrabber.getWidth()*vidGrabber.getHeight(); i++) {
			
			ofFloatColor sampleColor(vidGrabber.getPixels()[i * 3] / 255.f,				// r
				vidGrabber.getPixels()[i * 3 + 1] / 255.f,			// g
				vidGrabber.getPixels()[i * 3 + 2] / 255.f);			// b
				
																	
			ofVec3f tmpVec = mainMesh.getVertex(i);
			float audioValue = AudioData[i];
			
			ofVec3f direction = tmpVec.getNormalized();
			
				tmpVec = tmpVec + direction*(audioValue / 4)*(-extrusionAmount);// *extrusionAmount;
				tmpVec *= (fftLive.getThreshold());
				mainMesh.setVertex(i, tmpVec);
				mainMesh.setColor(i, sampleColor);

		}
		delete[] AudioData;
	}
		else if (ofGetElapsedTimef()==15) {
			ofSetBackgroundColor(255, 255, 255);
		}
		
		else if (ofGetElapsedTimef() > 15 && ofGetElapsedTimef() < 30) {
			vidGrabber.update();
			/*fftLive.setThreshold(audioThreshold);
			fftLive.setPeakDecay(audioPeakDecay);
			fftLive.setMaxDecay(audioMaxDecay);
			fftLive.setMirrorData(audioMirror);*/
			fftLive.update();
			ofPixels pixels = vidGrabber.getPixels();
			float * AudioData = new float[w*h * 3];
			float * AudioData2 = new float[w*h * 3];
			fftLive.getFftPeakData(AudioData2, w*h * 3);
			fftLive.getFftPeakData(AudioData, w*h * 3);
			for (int i = 1; i < (w*h * 3); i++) {

				float valeur = AudioData[i];
				float valeur2 = AudioData2[i];
				//fftLive.getFftData(noiseScale, i);

				float time = ofGetElapsedTimef();
				float noise = ofNoise(time + i * 0.001) * (valeur * 10);
				float noiseAmp = noise * amplitude;
				float waveform = sin((i * wavelength) + time * Speedslider);
				float wave = (waveform + valeur*10) * noiseAmp;
				int oWave = wave + i;
				output1[i] = pixels[oWave];
			}
			videoTexture.loadData(output1, w, h, GL_RGB);
		}
		else if (ofGetElapsedTimef() == 30) {
			ofResetElapsedTimeCounter();
		}
		else if (ofGetElapsedTimef() > 30) {
			vidGrabber.update();
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

			for (int i = 1; i<numOfVerts; i++) {
				ofFloatColor sampleColor(vidGrabber.getPixels()[i] / 255.f);		// b
				
				float audioValue = audioData[i];
				ofVec3f & vertOriginal = vertsOriginal[i];
				ofVec3f & vertWarped = vertsWarped[i];

				ofVec3f direction = vertOriginal.getNormalized();
				vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
				meshWarped.setVertex(i, vertWarped);
				//Pour afficher de l'info dans la console noire
				//cout << "Index : " << meshWarped.lenght << '\n';
				//cout << "Index : " << sampleColor << '\n';*/
				ofColor Color = sampleColor;

				//meshWarped.setColor(i,rouge);

			}

			delete[] audioData;
		}
		/*{
			vector<ofVec3f> & vertsOriginal = meshOriginal.getVertices();
			vector<ofVec3f> & vertsWarped = meshWarped.getVertices();
			int numOfVerts = meshOriginal.getNumVertices();

			float * audioData = new float[numOfVerts];
			fftLive.getFftPeakData(audioData, numOfVerts);

			float meshDisplacement = 100;

			for (int i = 0; i<numOfVerts; i++) {
				float audioValue = audioData[i];
				ofVec3f & vertOriginal = vertsOriginal[i];
				ofVec3f & vertWarped = vertsWarped[i];

				ofVec3f direction = vertOriginal.getNormalized();
				vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
			}

			delete[] audioData;
		}*/
	}
	


	//let's move the camera when you move the mouse
	float rotateAmount = 0;


	//move the camera around the mesh
	ofVec3f camDirection(0, 0, 1);
	ofVec3f centre(vidGrabber.getWidth() / 2.f, vidGrabber.getHeight() / 2.f, 255 / 2.f);
	ofVec3f camDirectionRotated = camDirection.getRotated(rotateAmount, ofVec3f(1, 0, 0));
	ofVec3f camPosition = centre + camDirectionRotated * extrusionAmount;

	cam.setPosition(camPosition);
	cam.lookAt(centre);


}

//--------------------------------------------------------------
void ofApp::draw() {
	//we have to disable depth testing to draw the video frame
	ofDisableDepthTest();
	//	vidGrabber.draw(20,20);

	//but we want to enable it to show the mesh
	ofEnableDepthTest();

	if (ofGetElapsedTimef() < 15) {
		cam.begin();
	//You can either draw the mesh or the wireframe

	mainMesh.drawFaces();
	cam.end();
}
	else if (ofGetElapsedTimef() == 15) {
		ofSetBackgroundColor(0, 0, 0);
	}
	else if (ofGetElapsedTimef() > 15 && ofGetElapsedTimef() < 30) {
		//vidGrabber.draw(w, h);
		videoTexture.draw(0, 0, w * 4, h * 4);
		wuv.draw();

	}
	else if (ofGetElapsedTimef() == 30) {
		ofSetBackgroundColor(0, 0, 0);
	}
	else if (ofGetElapsedTimef() > 30) {
		int w = OFX_FFT_WIDTH;
		int h = OFX_FFT_HEIGHT;
		int x = 20;
		int y = ofGetHeight() - h - 20;

		//video = webcam.getPixels();
		paf.loadData(video, x, y, GL_RGB);
		ofSetColor(255);
		vidGrabber.draw(0, 0, 320, 240);
		cam.begin();
		vidGrabber.bind();
		meshWarped.drawWireframe();
		meshWarped.drawFaces();
		vidGrabber.unbind();
		cam.end();
		gui.draw();
	}

	

	//draw framerate for fun
	ofSetColor(255);
	string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(msg, 10, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	}

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
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

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
