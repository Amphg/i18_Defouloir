# Defouloir


### Defouloir installation

This project is all about creating a space where people can be the loudest as possible, modifying their own image using the sound they make and getting in return a physical object that testify it. Ideally, it would be placed near a quiet space so that two spaces with the same function (i.e resting) can complete each other ; one is about resting quietly and the other resting by unleashing the loudest sound we can. The user would produce a sound and the computer would analyze it using Fast Fourier Transformations algorithms, using sound data as a parameter to modify the camera stream (filming the user and showing it as his reflexion).

### Prerequisites

To make the demo works, you will just need a computer with a webcam and a microphone in it

### Installing

To make this project run you need the following :

  * Openframeworks's last version 0.9.8 (http://openframeworks.cc/download/) 
  * ofxFFT-master library (https://github.com/julapy/ofxFFT)
    
    
### Steps

The demo is divided into 2 projects folder as the objectives of the demo is more of a technical experimentation of using sound data on a live webcam stream.

#### Effet de Sphere 3D

The idea is to initialize a 3D sphere mesh. For that, openframeworks has a powerful way to initialize a mesh as a sphere directly stocking a mesh we called meshWarped in : 
 * ```meshWarped = ofMesh::sphere(250 ,20);```
The first argument being the radius and the second modifying the sphere structure.
The mesh is made of different vertices which coordinates are stocked inside the sphere method. 

Then it's time to initialize the ofxFFT library that is packed with an easy way to get sound data as it allows the computer to get sound data from the webcam's microphone soundstream.
```fftLive.setup(); ```

Now, in the update function, the point is to get the coordinates of each vertices and stock it into an array of vectors so that we can access each of them using their index. This way we can manipulate their coordinates since they are vectors, using sound data so we can throw the updated vector coordinates into the original vertices coordinates.

 Firstly, we have to create two vectors, each of them stocking the vertices's coordinates in x,y and z.
    ```
    vector<ofVec3f> & vertsOriginal = meshOriginal.getVertices();
    vector<ofVec3f> & vertsWarped = meshWarped.getVertices();
    ```
 
 Then, we have to put the number of vertices into an integer to create an array with the length of that number of vertices (so that audio data can be assigned to modify each vertices). The ```fftLive.getFFtPeakData(float*,int);``` line permits to stock audio data inside an array of float pointers. We also have to clear the colors of the mesh as the update function is like the draw function, it repeats itself. We also have to resize the sampleColor vector so it has the length of the number of verticies.
 
 ```
  meshWarped.clearColors();
 int numOfVerts = meshWarped.getNumVertices();
float * audioData = new float[numOfVerts];
fftLive.getFftPeakData(audioData, numOfVerts);
  ```
  
 Finally, the mesh displacement happens using a ```for()``` loop. The loop goes through every vertices of the mesh and change its coordinates depending on the audioValue float. Then, we set the new coordinates of the warped mesh and the new color grabbed from webcam pixels - which actually does not work because of a problem with ```meshWarped.setColors``` method. #####Update, it does work now. I found a solution.
 The solution add the next lines ; first we need to initialize our two vectors vertOriginal and vertWarped here and not in the loop and give them placeholder values. Secondly, we add an array of color in the mesh with the size of sampleColor.
 ```
 ofVec3f & vertOriginal = vertsOriginal[0];
ofVec3f & vertWarped = vertsWarped[0];
 meshWarped.addColors(sampleColor);
 
 ```
 We can now add the updated version of the color grabber. As for an explanation, webcam's pixels are stored in an array and rgb value are stored in a line, thus having the following indexes in an array for 1 pixel ; red value : i | green value : i+1 | blue value : i+2
 
 ```
for (int i = numOfVerts; i>=0; i--) {
	float audioValue = audioData[i];
	vertOriginal = vertsOriginal[i];		// Old version ofVec3f & vertOriginal = vertsOriginal[i];
	vertWarped = vertsWarped[i];		// Old version ofVec3f & vertWarped = vertsWarped[i];
	//We add the color grabber. 
		sampleColor[i].r = webcam.getPixels()[i*3] / 255.f;
		sampleColor[i].g = webcam.getPixels()[(i*3  + 1)] / 255.f; 
		sampleColor[i].b = webcam.getPixels()[(i*3 + 2)] / 255.f;
	ofVec3f direction = vertOriginal.getNormalized();
	vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
	meshWarped.setVertex(i, vertWarped);
        meshWarped.setColors(i, sampleColor);				
	}
```
Finally, the last line of update that permits to start with a fresh new audio data array after each iteration.

```
delete[] audioData;
```

Then, we just need to put in the draw function everything that we need to make the mesh and the webcam appear, binding its data on the mesh.

```
webcam.draw(0, 0, 320, 240);
	cam.begin();
	meshWarped.drawFaces();
	cam.end();
```


#### Effet de distorsion

This second type of effect was the first try at modifying the webcam stream using audio data.
We need to setup our file properly in the ofApp.cpp with variables declared in ofApp.h

```
fftLive.setup();
vidGrabber.setup(w, h);
vidGrabber.setVerbose(true);
int w = 320;
int h = 240;
output1 = new unsigned char[w*h*3];
ofTexture videoTexture.allocate(w, h, GL_RGB);
ofTexture videoTexture1.allocate(w, h, GL_LUMINANCE);
int width = vidGrabber.getWidth();
int height = vidGrabber.getHeight();

//Mesh setup
for (int y = 0; y < height; y++) {
		for (int x = 0; x<width; x++) {
			mainMesh.addVertex(ofPoint(x, y, 0));	

			mainMesh.addColor(ofFloatColor(0, 0, 0));  
		}
	}
//Accessing the mesh vertex easily, "maping" vertex's index to the corresponding webcam pixel.
	for (int y = 0; y<height - 1; y++) {
		for (int x = 0; x<width - 1; x++) {
			mainMesh.addIndex(x + y*width);				
			mainMesh.addIndex((x + 1) + y*width);			
			mainMesh.addIndex(x + (y + 1)*width);			

			mainMesh.addIndex((x + 1) + y*width);		
			mainMesh.addIndex((x + 1) + (y + 1)*width);		
			mainMesh.addIndex(x + (y + 1)*width);			
		}
	}
int extrusionAmount = 3;
```

Now we can update everything we set up.
```
fftLive.update();
vidGrabber.update();
float * AudioData = new float[vidGrabber.getWidth()*vidGrabber.getHeight()]; // Just like the previous exemple, we need to put our audio data somewhere.
fftLive.getFftPeakData(AudioData, vidGrabber.getWidth()*vidGrabber.getHeight()); 
```

We can now extrude the mesh, browsing through every pixels of the webcam since the mesh and the webcam surface are the same.

```
for (int i = 0; i<vidGrabber.getWidth()*vidGrabber.getHeight(); i++) {
			
ofFloatColor sampleColor(
vidGrabber.getPixels()[i * 3] / 255.f,		
vidGrabber.getPixels()[i * 3 + 1] / 255.f,			
vidGrabber.getPixels()[i * 3 + 2] / 255.f);			
				
																	
ofVec3f tmpVec = mainMesh.getVertex(i);
float audioValue = AudioData[i];
ofVec3f direction = tmpVec.getNormalized();
tmpVec = tmpVec + direction*(audioValue / 4)*(-extrusionAmount);// *extrusionAmount;
tmpVec *= (fftLive.getThreshold());
mainMesh.setVertex(i, tmpVec);
mainMesh.setColor(i, sampleColor);
		}
delete[] AudioData;
```

Another effect using noise can be used in the same update function. We can just put a condition using ``` ofGetElapsedTime() ``` to just go through every effects. First, we need to create some array to store our values.

```
fftLive.update();
ofPixels pixels = vidGrabber.getPixels();
float * AudioData = new float[w*h * 3];
float * AudioData2 = new float[w*h * 3];
fftLive.getFftPeakData(AudioData2, w*h * 3);
fftLive.getFftPeakData(AudioData, w*h * 3);
```

Then we just need to distort the webcam pixels stored in pixels. Just as a precision, "amplitude", "noise", "wavelength" and the other named value are initialized as arbitrary values in the ofApp.h.

```
for (int i = 1; i < (w*h * 3); i++) {
float valeur = AudioData[i];
float valeur2 = AudioData2[i];
		
float time = ofGetElapsedTimef();
float noise = ofNoise(time + i * 0.001) * (valeur * 10);
float noiseAmp = noise * amplitude;
float waveform = sin((i * wavelength) + time * Speedslider);
float wave = (waveform + valeur*10) * noiseAmp;
int oWave = wave + i;
output1[i] = pixels[oWave];
			}
videoTexture.loadData(output1, w, h, GL_RGB); // We load the pixels valors stored in the video texture.
```

Finally, we just have to finish by calling everything in the draw function.

```
ofDisableDepthTest();
ofEnableDepthTest();
if (ofGetElapsedTimef() < 15) { //First effect
cam.begin();
mainMesh.drawFaces();
cam.end();
	}
else if (ofGetElapsedTimef() > 15 && ofGetElapsedTimef() < 30) { //Second effect
videoTexture.draw(0, 0, w * 4, h * 4);
wuv.draw();
}
```

And that's pretty much it.
