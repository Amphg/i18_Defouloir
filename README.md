# Defouloir


### Defouloir installation

This project is all about creating a space where people can be the loudest as possible, modifying their own image using the sound they make and getting in return a physical object that testify it. Ideally, it would be placed near a quiet space so that two spaces with the same function (i.e resting) can complete each other ; one is about resting quietly and the other resting by unleashing the loudest sound we can. The user would produce a sound and the computer would analyze it using Fast Fourier Transformations algorithms, using sound data as a parameter to modify the camera stream (filming the user and showing it as his reflexion).

### Prerequisites

To make the demo works, you will just need a computer with a webcam and a microphone in it

### Installing

To make this project run you need the following :

Openframeworks's last version 0.9.8 (http://openframeworks.cc/download/) 
ofxFFT-master library (https://github.com/julapy/ofxFFT)
    
    
### Steps

The demo is divided into 2 projects folder as the objectives of the demo is more of a technical experimentation of using sound data on a live webcam stream.

#### Effet de Sphere 3D```

The idea is to initialize a 3D sphere mesh. For that, openframeworks has a powerful way to initialize a mesh as a sphere directly stocking a mesh we called meshWarped in : 
```meshWarped = ofMesh::sphere(250 ,20);```
The first argument being the radius and the second modifying the sphere structure.
The mesh is made of different vertices which coordinates are stocked inside the sphere method. 

Then it's time to initialize the ofxFFT library that is packed with an easy way to get sound data as it allows the computer to get sound data from the webcam's microphone soundstream.
```fftLive.setup(); ```

Now, in the update function, the point is to get the coordinates of each vertices and stock it into an array of vectors so that we can access each of them using their index. This way we can manipulate their coordinates since they are vectors, using sound data so we can throw the updated vector coordinates into the original vertices coordinates.

