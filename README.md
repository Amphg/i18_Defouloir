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

#### Effet de Sphere 3D```

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
 
 Then, we have to put the number of vertices into an integer to create an array with the length of that number of vertices (so that audio data can be assigned to modify each vertices). The ```fftLive.getFFtPeakData(float*,int);``` line permits to stock audio data inside an array of float pointers.
  ```
 int numOfVerts = meshWarped.getNumVertices();
	float * audioData = new float[numOfVerts];
	fftLive.getFftPeakData(audioData, numOfVerts);
  ```
  
 Finally, the mesh displacement happens using a ```for()``` loop. The loop
 ```
for (int i = numOfVerts; i>=0; i--) {
       //ofFloatColor sampleColor(webcam.getPixels()[i] / 255.f;	 webcam.getPixels()[i + 1] / 255.f;	 webcam.getPixels()[i + 2] / 255.f; );	// b
	float audioValue = audioData[i];
	ofVec3f & vertOriginal = vertsOriginal[i];
	ofVec3f & vertWarped = vertsWarped[i];
	ofVec3f direction = vertOriginal.getNormalized();
	vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
	meshWarped.setVertex(i, vertWarped);
      //meshWarped.setColors(i, sampleColor);
		

		
	}```

