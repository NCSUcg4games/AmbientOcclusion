/*
   Irena Rindos

   Image class- reads in camera, sets up viewscreen, and creates pixmap. 
*/

#ifndef __IMAGE__
#define __IMAGE__

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "color.h"
#include "ray.h"
#include "scene.h"
using namespace std;

//Default width and height of window
#define WIDTH 250
#define HEIGHT 200
#define DFAR 1.0
#define ANTIALIAS 3
#define AOSAMPLES 256
#define SSAO 16

class PixImage{
public:
   PixImage();
   ~PixImage(){delete(pixmap);}

   void createPixels();
   int testPixwidth(char* argv);
   int handleArgs(int argc, char* argv[]);
   int loadCamera(); //Load camera
   void calcViewCoords(); //Calculate cordinates and viewscreen based on camera
	 bool withinDistance(Vector3d a, Vector3d b);
	 Vector3d genHemisphereRay(Vector3d normal);
	 void createAOMap();
	 void createSSAOMap();
   void createImage();
	 void composite();

   ColorPixCh *pixmap; 
   ColorPixCh *aomap;
	 ColorPixCh *ssaomap;
   ColorPixCh *aoCombmap;
	 ColorPixCh *ssaoCombmap;

	 Collision  *collisionBuffer;
	 Scene scene;
   int pixwidth, pixheight;

   //Arbitrary camera attributes read in from file
   Ray camera;
   Vector3d camVup;
   float viewW, viewH, aspRatio, focalL; 

   //Camera coordinates, viewscreen center, and lower left of viewscreen
   Vector3d Ux, Uy, Uz, viewC, lowerLft;
   string filename;
};

#endif
