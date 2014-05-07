/*
   Irena Rindos 

   Collision class- used to store various information relevant to collision for use in shading, etc.
*/
#ifndef __COLLISION__
#define __COLLISION__

#include <cstdlib>
#include <iostream>

#include "Vector.h"
#include "material.h"
#include "color.h"


class Collision {
public:
  Collision() {hit = false;};
  bool hit;
	bool isSphere;
  double distance;
  Material material;
  Vector3d hitPnt;
  Vector3d surfNorm;
};

#endif
