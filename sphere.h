/*
   Irena Rindos 	

   Sphere class- tests for intersections
*/

#ifndef __SPHERE__
#define __SPHERE__

#include <cstdlib>
#include <iostream>

#include "Vector.h"
#include "color.h"
#include "ray.h"
#include "material.h"
#include "collision.h"

class Sphere {
public:
   Sphere() {};
   void init(double x, double y, double z, double r, ColorPix c, ColorPix s, double nSpec);
   Collision testIntersect(Ray ray);
   Material getMaterial() {return material;}

private:
  Vector3d center;
  double radius;
  Material material;
};


#endif
