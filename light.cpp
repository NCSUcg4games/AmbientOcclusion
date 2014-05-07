#include "light.h"

void Light::initPnt(ColorPix c, Vector3d l){
  color = c; 
  location= l;
  isPnt = true;
  isPrl = false;
}

void Light::initPrl(ColorPix c, Vector3d d){
  color = c; 
  direction= d;
  isPnt = false;
  isPrl = true;
}
