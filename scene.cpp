#include "scene.h"

void Scene::create() {
//Point light
   Vector3d lb(-1.0,1.0,0.25);
   ColorPix plBlue(0.4,0.4,0.8,1.0);
   light[0].initPnt(plBlue,lb);

//Sun: 45 degrees from z going right to left, 60 degrees from the x-z plane
//Use spherical coords with radius 1; theta is 60, phi is 45
   double rad = M_PI/180;//multiply degrees by this 
 // Vector3d dir(-(cos(60*rad)*sin(45*rad)), -(sin(60*rad)*sin(45*rad)),-cos(45*rad));
   Vector3d dir(-1, -1.7, -1);//direction of sun via email
   dir = dir.normalize();
   ColorPix plYlw(0.8,0.8,0.2,1.0);
   light[1].initPrl(plYlw,dir);

//Spheres
   ColorPix red(1.0,0,0,1.0);	
   ColorPix green(0,1.0,0,1.0);
   ColorPix blue(0,0,1.0,1.0);
   ColorPix orange(1.0,.5,0,1.0);
   ColorPix purple(.5,0,.5,1.0);
   ColorPix white(1.0,1.0,1.0,1.0);

  //The following are spec colors- since a black spec will not show up (mult. by 0),
  //use that for a fully diffuse surface
  ColorPix matte(0,0,0,1);
  ColorPix shiny(1,1,1,1);
  ColorPix mix(.75, .75, .75, 1);//between matte and shiny

   double nSpec = 70.0;
   double broadSpec = 40.0;

   spheres[0].init(-.30, 0.1, -0.5, 0.05, red, matte,nSpec);
   spheres[1].init(0.0, -0.2, -0.7, 0.15, green,shiny, nSpec);
   spheres[2].init(.3, .3, -.45, 0.3, blue, mix, broadSpec);
   spheres[3].init(-0.2, -0.25, -0.4, 0.225, purple,matte,nSpec);

//Planes
   Vector3d n1(1,0,0);
   Vector3d n2(-1,0,0);
   Vector3d n3(0,1,0);
   Vector3d n4(0,-1,0);
   Vector3d n5(0,0,1);
   Vector3d p1(-1, 1, -3);
   Vector3d p2(1,1, -3);
   Vector3d p3(1, -1, -3);
   Vector3d p4(1, 1, -3);
   Vector3d p5(0, 0, -.85);

   double pSpec = 300.0;
   planes[0].init(p5, n5, white, matte, pSpec);
/*   planes[1].init(p2, n2, white, matte, pSpec);
   planes[2].init(p3, n3, white, matte, pSpec);
   planes[3].init(p4, n3, white, matte, pSpec);
   planes[4].init(p5, n5, white, matte, pSpec);
*/
}

ColorPix Scene::lightCalc(Collision collision, Ray ray){ 
   ColorPix finalCol(0,0,0,1);
   ColorPix ambCol(0,0,0,1);
   ColorPix diffCol(0,0,0,1);
   ColorPix specCol(0,0,0,1);

//Ambient
   ambCol = collision.material.ambCol * .05;

  int i;
  for(i =0; i < NOLIGHTS; i++) {
//Diffuse
      Vector3d cl;
      if(light[i].isPnt) cl = collision.hitPnt - light[i].location; 
      if(light[i].isPrl)  cl =  light[i].direction;
      cl = cl.normalize();

      double diffuse = -cl * collision.surfNorm;
      if(diffuse > 0.0){
	diffCol = diffCol + ((collision.material.diffCol * diffuse)* light[i].color);

//Specular
	//Reflect light around the normal  
	Vector3d specVec = cl - 2*(cl * collision.surfNorm)*collision.surfNorm;
	specVec= specVec.normalize();
	double specCos = -specVec * ray.unitDir; //get angle between it and ray dir 
						 //(my ray goes from eye to pnt, so negate)
	if(specCos > 0.0){
	   double spec = pow(specCos, collision.material.n);
	   specCol = specCol + ((light[i].color * spec) * collision.material.specCol);
	}
      }
   }
   finalCol= ambCol + diffCol + specCol;
   return finalCol;
}

ColorPix Scene::testIntersections(Ray ray, Collision &c) {
//Set color to background
   ColorPix closestColor(0,0,0,1.0);

   int i;
   double closestDist;
   Collision collision, test;
   closestDist = CLIPPLANE;

//Find object is closer to the camera
   for(i=0; i < NOSPHERES; i++){
			test= spheres[i].testIntersect(ray);
			if(test.hit && (test.distance < closestDist)){//&& test.distance >BIAS){
				 closestDist= test.distance;
				 collision = test;
				 collision.isSphere=true;
			}
   }

   for(i=0; i < NOPLANES; i++){
			test= planes[i].testIntersect(ray);
			if(test.hit && (test.distance < closestDist)){/// && test.distance >BIAS){
				 closestDist= test.distance;
				 collision = test;
				 collision.isSphere=false;
			}
   }
//If no hit, just return black
   if(collision.hit){
			closestColor = lightCalc(collision, ray);
   }
	 c= collision;
   return closestColor;
}
