/*
   Irena Rindos

   Light class- can be used for a point or parallel light 
*/

#ifndef __LIGHT__
#define __LIGHT__

#include <cstdlib>
#include <iostream>

#include "Vector.h"
#include "color.h"

class Light {
public:
  Light() {};
  void initPnt(ColorPix c, Vector3d l);
  void initPrl(ColorPix c, Vector3d d);
  ColorPix color;
  Vector3d location, direction;
  bool isPnt, isPrl; //identifies sources as point or parallel source
};

#endif
