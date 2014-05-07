/*
   Irena Rindos 	

   Scene class- sets up the scene and runs through objects for collisions
*/

#ifndef __SCENE__
#define __SCENE__

#include <cstdlib>
#include <iostream>

#include "sphere.h"
#include "plane.h"
#include "ray.h"
#include "color.h"
#include "collision.h"
#include "material.h"
#include "light.h"

#define NOPLANES 1
#define NOSPHERES 4
#define NOLIGHTS 2
#define CLIPPLANE 999999999
#define BIAS .01

class Scene{
public:
   Scene() {};
   void create();
   ColorPix lightCalc(Collision collision, Ray ray);
   ColorPix testIntersections(Ray ray, Collision &c);

private:
   Sphere spheres[NOSPHERES];
   Light light[NOLIGHTS];
   Plane planes[NOPLANES];
};

#endif
