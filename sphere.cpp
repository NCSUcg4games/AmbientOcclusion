#include "sphere.h"

void Sphere::init(double x, double y, double z, double r, ColorPix c, ColorPix s, double nSpec) { 

   center.x= x; 
   center.y= y; 
   center.z= z;
 
   radius = r;
   material.ambCol= c;
   material.diffCol= c;

   material.specCol=  s;
   material.n = nSpec;
}

Collision Sphere::testIntersect(Ray ray) { 
  Collision collision;

   Vector3d toCenter = (center-ray.origin);                //create a ray from the sphere center
   double tclose = toCenter * ray.unitDir;  //Length of Projection of that onto the ray vector 
   Vector3d xclose = ray.origin + (tclose * ray.unitDir);   //Position along ray to hit pnt
   Vector3d distance = xclose - center;

   double mag = distance.norm();//If the magnitude of ray > radius, the ray does not hit

   if(mag > radius){
		collision.hit = false;
		return collision;  
   }
//If the distance is exacly equal to radius, there is only one hit
   if (mag == radius){            //and the distance equals the length of the ray	
			collision.distance = mag;
      collision.hitPnt = xclose; 
   }
//Otherwise there are two hits; find the closest one
   else{
			double a = sqrt((radius * radius)-(mag * mag));
			Vector3d xhit = ray.origin + ((tclose -a)* ray.unitDir);
			collision.distance = xhit.norm();
      collision.hitPnt = xhit;
   }
   Vector3d temp =collision.hitPnt - center; 
   collision.surfNorm = temp.normalize();
   collision.material = material;
   collision.hit = true;
   return collision;
}
