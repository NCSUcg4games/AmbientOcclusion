/*
   Irena Rindos 	

   ColorPix class- used for adding colors, multiplying two colors, multiplying a color by a scalar,
   and clamping color values
*/

#ifndef __COLOR__
#define __COLOR__


#include <cstdlib>
#include <iostream>

class  ColorPix {
public:
  ColorPix(){};
  ColorPix(double r, double g, double b, double a);

  void reset(); //set to 0s

  void clamp(); //test for over 1, etc
  ColorPix operator*(double s); //multiply color by a factor
  ColorPix operator*(const ColorPix& b); //multp colors
  ColorPix operator+(const ColorPix& b); //add colors
  ColorPix operator+(double s); //adding to colors

  double R, G, B, A;
};

class  ColorPixCh {
public:
  ColorPixCh(){};
  ColorPixCh(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
  void convert(ColorPix p);
  unsigned char R, G, B, A;
  ColorPixCh operator*(ColorPixCh& b); //multp colors
};

#endif


