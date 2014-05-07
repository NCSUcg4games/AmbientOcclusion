#include "ray.h"

Ray::Ray() :
   origin(0,0,0),
   direction(0,0,0)
{}

Ray::Ray(double x, double y, double z) : 
   origin(x,y,z),
   direction(0,0,0)
{}

Ray::Ray(Vector3d o, Vector3d d, Vector3d u):
	origin(o),
	direction(d),
	unitDir(u)
{}
