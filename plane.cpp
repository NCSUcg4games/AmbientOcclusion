#include "plane.h"

void Plane::init(Vector3d p, Vector3d n, ColorPix c, ColorPix s, double nSpec){
   point = p;
   normal = n;

   material.ambCol= c;
   material.diffCol= c;

   material.specCol=  s;
   material.n = nSpec;
}

void Plane::setVertices(Vector3d a, Vector3d b, Vector3d c, Vector3d d){
   vertex[0]= a;
   vertex[1]= b;
   vertex[2]= c;
   vertex[3]= d;
}
 
void Plane::setMaterial(ColorPix c, ColorPix s){
   material.ambCol= c;
   material.diffCol= c;
   material.specCol=  s;
   material.n = 300.00;
}

Collision Plane::testIntersect(Ray ray) {
  Collision collision;

//create a ray from the point to the ray
   Vector3d pR = (point-ray.origin);

   double t; //t value in ray eq where it intersects the plane
   t = (normal * pR)/(normal* ray.unitDir);

//calculate hit point!!

   if(t > 0) {
	collision.distance = t;
	collision.material = material;
        collision.surfNorm = normal;
	collision.hitPnt = ray.origin + (t * ray.unitDir);
   	collision.hit = true;
   }
   return collision;
}
