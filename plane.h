/*
   Irena Rindos 	

   Plane class- calculates plane intersections
*/

#ifndef __PLANE__
#define __PLANE__

#include <cstdlib>
#include <iostream>

#include "Vector.h"
#include "color.h"
#include "ray.h"
#include "material.h"
#include "collision.h"

class Plane {
public:
   Plane() {};
   void init(Vector3d p, Vector3d n, ColorPix c, ColorPix s, double nspec);
   void setVertices(Vector3d a, Vector3d b, Vector3d c, Vector3d d);
   Collision testIntersect(Ray ray);
   void setMaterial(ColorPix c, ColorPix s);
   void setPoint(Vector3d p){point = p;}
   Material getMaterial() {return material;}
//   Vector3d getNormal() {return normal;}
   void setNormal(Vector3d norm) {normal = norm;}

  Vector3d vertex[4];
  Vector3d normal;

private:
  Vector3d point;
  Material material;
};
#endif
