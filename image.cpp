#include "image.h"

PixImage::PixImage():
   pixwidth(WIDTH),
   pixheight(HEIGHT),
   filename("null")
{}

void PixImage::createPixels(){
  pixmap=new ColorPixCh[pixwidth * pixheight];
	aomap=new ColorPixCh[pixwidth * pixheight];
	ssaomap=new ColorPixCh[pixwidth * pixheight];
	aoCombmap=new ColorPixCh[pixwidth * pixheight];
	ssaoCombmap=new ColorPixCh[pixwidth * pixheight];
  collisionBuffer = new Collision[pixwidth * pixheight];
}

int PixImage::testPixwidth(char* argv){
   string pix = (string)argv;

   int i;
//Go through to see if the whole argv is a number
   for(i= 0; i < pix.size(); i++){
	if(!isdigit(pix[i])) return 0;
   }
   istringstream convert(pix);
   convert >> pixwidth;

   pixheight = (4 * pixwidth) / 5;

   return 1; //valid pixwidth
}

int PixImage::handleArgs(int argc, char* argv[]){
  if(argc == 2){
		if(testPixwidth(argv[1])) return 0;
  }

  return 1;//Failure
}

int PixImage::loadCamera(){
   ifstream cameraF;
   double locX, locY, locZ; //camera location
   double dirX, dirY, dirZ; //camera direction
   double vupX, vupY, vupZ; //vup vector
   double focal, ratio, screenW; //focal length, aspect ratio, screen width

   cameraF.open("camera.txt");
   if(!cameraF.is_open()) return 1; //open failed, return failure

   cameraF >> skipws >> locX >> locY >> locZ >> dirX >> dirY >> dirZ >>vupX >> vupY >> vupZ
	>> focal >> ratio >>screenW;

   camera.origin.x = locX;
   camera.origin.y = locY;
   camera.origin.z = locZ;

   camera.direction.x =dirX;
   camera.direction.y =dirY;
   camera.direction.z =dirZ;

   camVup.x = vupX;
   camVup.y = vupY;
   camVup.z = vupZ;

   focalL = focal;
   aspRatio = ratio;
   viewW = screenW;
   viewH = screenW *(1.0/aspRatio);

   calcViewCoords();

   return 0;
}

void PixImage:: calcViewCoords(){
//Coords. for arbitrary camera
   Uz = -camera.direction.normalize();

   Ux = camera.direction % camVup;
   Ux = Ux.normalize();

   Uy = Uz % Ux;
   Uy = Uy.normalize(); 

   //Center of viewscreen and the lower left corner of the viewscreen
   viewC = camera.origin - (focalL * Uz); 
   lowerLft = viewC - ((viewW/2.0) * Ux) - ((viewH/2.0) * Uy);
}

bool PixImage:: withinDistance(Vector3d a, Vector3d b){
	float distance = sqrt(
									((a.x-b.x)*(a.x-b.x))+
									((a.y-b.y)*(a.y-b.y))+
									((a.z-b.z)*(a.z-b.z))
									);
	if(distance < DFAR) return true;
	else return false;
}

//Adapted from GPU gems: http://http.developer.nvidia.com/GPUGems/gpugems_ch17.html
//Example 17-2. Algorithm for Computing Random Directions with Rejection Sampling
Vector3d PixImage:: genHemisphereRay(Vector3d normal){
		while (true) {
			srand(time(0));
			Vector3d aoRay((drand48()*2)-1,(drand48()*2)-1,(drand48()*2)-1 );
			if (aoRay.x * aoRay.x + aoRay.y * aoRay.y + aoRay.z * aoRay.z > 1) continue; // ignore ones outside unit
							                                 
			 // sphere
			 if ((aoRay*normal) < 0) continue; // ignore "down" dirs

			 return aoRay.normalize(); // success!
		}
}

//Create composite images with the AO & SSAO
void PixImage::composite(){
   for(int i= 0; i< pixheight; i++){
			for(int j=0; j< pixwidth; j++){
				//
				int currPix = (i*pixwidth) + j;
				ColorPixCh ao, ssao;
				ao= aomap[currPix ] *pixmap[currPix];
				ssao= ssaomap[currPix ] *pixmap[currPix ];

				aoCombmap[currPix ]= ao;
				ssaoCombmap[currPix ]= ssao;
		}
	}
}

//Create AO data as a grayscale image
void PixImage:: createAOMap(){
   for(int i= 0; i< pixheight; i++){
			for(int j=0; j< pixwidth; j++){
				//Get our pnt
				Vector3d hit = collisionBuffer[(i*pixwidth) + j].hitPnt;
				Vector3d norm =  collisionBuffer[(i*pixwidth) + j].surfNorm;
				//move hit point up a bit to prevent self-collisions
				hit = hit+(.001*norm);
				int occlusionCount = 0;
				//Generate hemisphere around point
				for(int a=0; a<AOSAMPLES; a++){
					Vector3d aoVec = genHemisphereRay(norm);
					Ray aoRay(hit, aoVec, aoVec);						
					//cout<<"Ao ray is: "<<aoVec<<endl;
					//Cast ray, if there is a collision, check with DFAR distance
					Collision collision;
   				//collision.index = -1;
					scene.testIntersections(aoRay, collision);
					//If there was a hit... test against dfar
				  if(collision.hit){
						if(withinDistance(hit, collision.hitPnt))
							occlusionCount++;
					}
				}

				//Calculate occlusion for this pixel
				float occlusion =((float)occlusionCount / (float)AOSAMPLES);//1.0-(1.0/PI*(((float)occlusionCount / (float)AOSAMPLES)));
				if(!collisionBuffer[(i*pixwidth) + j].isSphere)occlusion= 1.00-occlusion;
				//cout<<"Ocll is: "<<occlusion<<endl;
 				ColorPix color(occlusion,occlusion,occlusion,1);
				ColorPixCh chColor;
		 	  chColor.convert(color); //Convert floating points to unsigned chars
				aomap[(i*pixwidth) + j]= chColor;
			}
   }
}

//Create SSAO data as a grayscale image
void PixImage:: createSSAOMap(){
//DFAR is far cutoff
//just look at near pixels for now?
   for(int i= 0; i< pixheight; i++){
			for(int j=0; j< pixwidth; j++){
				//Get our zvalue
				float z = collisionBuffer[(i*pixwidth) + j].hitPnt.z;
				Vector3d p = collisionBuffer[(i*pixwidth) + j].hitPnt;
				p = p+ (0.001 * collisionBuffer[(i*pixwidth) + j].surfNorm);
				float compareZ;
				//get one row around
				int neighbors = 0;
				int occlusionCount = 0;
				for(int k = SSAO; k>=-SSAO; k--){
					//K modifies i, check top and bottom
					if((i+k) < 0 || (i+k)>pixheight) continue;
					for(int l = SSAO; l>=-SSAO; l--){
						//l modifies j, check column
						if((l+j) <0|| (l+j)> pixwidth) continue;
						compareZ = collisionBuffer[((i+k)*pixwidth) + (j+l)].hitPnt.z;
					//	if(withinDistance(p, collisionBuffer[((i+k)*pixwidth) + (j+l)].hitPnt)){
							if(compareZ > z) occlusionCount++;
							
					//	}
						neighbors++;
					}
				}

				//Calculate occlusion for this pixel
				float occlusion =1.0-(1.0/PI*(((float)occlusionCount / (float)neighbors)));
				//cout<<"Ocll is: "<<occlusion<<endl;
 				ColorPix color(occlusion,occlusion,occlusion,1);
				 ColorPixCh chColor;
		 	   chColor.convert(color); //Convert floating points to unsigned chars
				ssaomap[(i*pixwidth) + j]= chColor;
			}
   }
}

void PixImage:: createImage(){
   int i, j, a, b;

  // Scene scene;
   scene.create();

   double dydi, dxdj;
   dydi= viewH / (double)pixheight;
   dxdj= viewW / (double)pixwidth;

   double x, y;

   for(i= 0; i< pixheight; i++){
			double camY = (i + .5) * dydi;
			for(j=0; j< pixwidth; j++){
				 ColorPix color(0,0,0,1);
				 Collision coll;
				 //Antialiasing loop
				 for (a=0; a< ANTIALIAS; a++){
					   for(b=0; b< ANTIALIAS; b++){
								 y= camY + (a/((double)ANTIALIAS * pixheight));
								 x = ((j + .5) *dxdj) +(b/((double) ANTIALIAS * pixwidth));
								 //Direction used to calculate the Ux/y/z, overwriting here since we have what we need
								 camera.direction = (lowerLft + (y*Uy) +(x*Ux)) - camera.origin;
								 camera.unitDir = camera.direction.normalize();

			 	   	 		 color = color + (scene.testIntersections(camera, coll) * (1.0/(ANTIALIAS * ANTIALIAS)));
							}
				 }
				 ColorPixCh chColor;
		 	   chColor.convert(color); //Convert floating points to unsigned chars
				 pixmap[(i*pixwidth) + j]= chColor; 
				 collisionBuffer[(i*pixwidth) + j]= coll;
				// cout<<"Collision z is: "<< coll.hitPnt<<endl;
			}
   }
	 createAOMap();
 	 createSSAOMap();
	 composite();
}

