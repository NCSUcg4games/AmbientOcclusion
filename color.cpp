#include "color.h"

ColorPix:: ColorPix(double r, double g, double b, double a):
  R(r), 
  G(g), 
  B(b), 
  A(a)
{}

void ColorPix:: reset() {
   R = 0.0;
   G = 0.0;
   B = 0.0;
   A = 1.0;
}

void ColorPix:: clamp() {
   if (R > 1.0 ) R = 1.0;
   if (G > 1.0 ) G = 1.0;
   if (B > 1.0 ) B = 1.0;
}

ColorPix ColorPix::operator*(double s){ //multiply color by a factor
   ColorPix temp;
   temp.R = R * s;
   temp.G = G * s;
   temp.B = B * s;
   temp.A = 1.0;

   return temp;
}

ColorPix ColorPix::operator*(const ColorPix& b){ //mult colors
   ColorPix temp;
   temp.R = R * b.R;
   temp.G = G * b.G;
   temp.B = B * b.B;
   temp.A = 1.0;

   return temp;
}

ColorPix ColorPix::operator+(const ColorPix& b){ //adding colors
   ColorPix temp;
   temp.R = R + b.R;
   temp.G = G + b.G;
   temp.B = B + b.B;
   temp.A = 1.0;

   return temp;
}

ColorPix ColorPix::operator+(double s){ //adding colors
   ColorPix temp;
   temp.R = R + s;
   temp.G = G + s;
   temp.B = B + s;
   temp.A = 1.0;

   return temp;
}

ColorPixCh:: ColorPixCh(unsigned char r, unsigned char g, unsigned char b, unsigned char a):
  R(r), 
  G(g), 
  B(b), 
  A(a)
{}

void ColorPixCh::convert(ColorPix p) {
   p.clamp();
   R = (int)(p.R *255);
   G = (int)(p.G *255);
   B = (int)(p.B *255);
   A = 255;
}

ColorPixCh ColorPixCh::operator*(ColorPixCh& b){ //mult colors, convert to float, then back
   ColorPix temp;
   temp.R = (R/255.0) * (b.R/255.0);
   temp.G = (G/255.0) * (b.G/255.0);
   temp.B = (B/255.0) * (b.B/255.0);
   temp.A = 1;
	 ColorPixCh t;
	 t.convert(temp);

   return t;
}

