/*
   Irena Rindos 

   Ray class- used for rays/ cameras
*/

#ifndef __RAY__
#define __RAY__


#include <cstdlib>
#include <iostream>

#include "Vector.h"


class  Ray {
public:
   Ray();
   Ray(double x, double y, double z);
	 Ray(Vector3d o, Vector3d d, Vector3d u);

//These are public since they will be used very often
   Vector3d origin;
   Vector3d direction;
   Vector3d unitDir;
};

#endif
