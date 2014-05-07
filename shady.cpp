/*
   Irena Rindos 
	 CSC 591 Final Project
	
	 AO & SSAO Raytracer & Compositor

   Comamandline format: raycast[pixwidth]
   pixwidth = variable pixwidth, default is 250
   Use "w" after image is raytraced to write image, AO, SSAO, and composites to files
*/

#include <Magick++.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "scene.h"
#include "plane.h"
#include "image.h"


using namespace std;
using namespace Magick;

PixImage pixmap;

void writeImage(){
 // if(pixmap.filename == "null") return;

	//Write the normal raytracer
	string normal="raytrace.png";
  Image out_image(pixmap.pixwidth, pixmap.pixheight, "RGBA", CharPixel, (void *)pixmap.pixmap);
  out_image.flip();
  out_image.write(normal);

	//Write the SSAO
  string ssao = "ssao.png";
  Image ssao_image(pixmap.pixwidth, pixmap.pixheight, "RGBA", CharPixel, (void *)pixmap.ssaomap);
  ssao_image.flip();
  ssao_image.write(ssao);

  //Write the AO
  string ao = "ao.png";
  Image ao_image(pixmap.pixwidth, pixmap.pixheight, "RGBA", CharPixel, (void *)pixmap.aomap);
  ao_image.flip();
  ao_image.write(ao);
	
	//Write the composites
  string aoC = "aoComp.png";
  Image aoC_image(pixmap.pixwidth, pixmap.pixheight, "RGBA", CharPixel, (void *)pixmap.aoCombmap);
  aoC_image.flip();
  aoC_image.write(aoC);

  string ssaoC = "ssaoComp.png";
  Image ssaoC_image(pixmap.pixwidth, pixmap.pixheight, "RGBA", CharPixel, (void *)pixmap.ssaoCombmap);
  ssaoC_image.flip();
  ssaoC_image.write(ssaoC);
}

void handleResize(int width, int height){

//In case a teeny window was made larger; this also keeps it centered
    if( height >=pixmap.pixheight && width >=pixmap.pixwidth){
	//The following so images don't disappear at large window dimensions
        glViewport(0,0,width,height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);
        glMatrixMode(GL_MODELVIEW);
	glPixelZoom(1.0, 1.0);
	glRasterPos2i((GLint)((width -pixmap.pixwidth)/2) , (GLint)((height- pixmap.pixheight)/2)); 
	return;
   } 

//Make given width and height of current window for pixelzoom
   float w = width;
   float h = height;
//Scale factors
   float ratioW = w / pixmap.pixwidth;
   float ratioH = h / pixmap.pixheight;

//Shrink the image proportionally if the window is too small & keep centered
   if( h < pixmap.pixheight|| w < pixmap.pixwidth){
	if (ratioW < ratioH){
	  glPixelZoom(ratioW, ratioW);
          glRasterPos2i(0, (GLint)(h-(ratioW * pixmap.pixheight))/2); 
	}
	else {
	   glPixelZoom(ratioH, ratioH);
           glRasterPos2i((GLint)(w-(ratioH * pixmap.pixwidth))/2, 0);
	}
   }   
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT);  // clear window to background color 
  glDrawPixels(pixmap.pixwidth, pixmap.pixheight, GL_RGBA, GL_UNSIGNED_BYTE, pixmap.pixmap);
  glutSwapBuffers();
}

void handleKey(unsigned char key, int x, int y){
  
  switch(key){

    case 'w': case 'W':
			writeImage();
	break;
      
    case 'q': case 'Q':  // q - quit
    case 27:		 // esc - quit
      exit(0);
      
    default:		// not a valid key -- just ignore it
      return;
  }
}

int main(int argc, char* argv[]){

//pass pixmap to this
   if(argc > 1){
     int usageError; 
     usageError = pixmap.handleArgs(argc, argv);
     if(usageError){
			cout << "usage: raycast [pixwidth]" << endl;
			return(1);
     }
   }
  if(pixmap.loadCamera()) {
		cout << "Camera file read failed" << endl;
	return(1);
  }

  pixmap.createPixels();

	pixmap.createImage();
  glutInit(&argc, argv);

  // create the graphics window, giving width, height, and title text
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(pixmap.pixwidth, pixmap.pixheight);
  glutCreateWindow("AO & SSAO");

  glutDisplayFunc(display);	  // display callback
  glutKeyboardFunc(handleKey);	  // keyboard callback
  glutReshapeFunc(handleResize);  // resize callback

  // define the drawing coordinate system on the viewport
  // lower left is (0, 0), upper right is (WIDTH, HEIGHT)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, pixmap.pixwidth, 0, pixmap.pixheight);
  glMatrixMode(GL_MODELVIEW);

  // specify window clear (background) color to black
  glClearColor(0, 0, 0, 0);
  glutMainLoop();

  return 0;
}
