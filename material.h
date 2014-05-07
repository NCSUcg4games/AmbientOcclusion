/*
   Irena Rindos 	

   Material class- used to store material colors and properties
*/

#ifndef __MATERIAL__
#define __MATERIAL__

#include <cstdlib>
#include <iostream>

#include "color.h"

class Material {
public:
  Material() {};
  Material(ColorPix col, ColorPix scol);
  ColorPix ambCol, diffCol, specCol; 
  double n; // spec coeff
};

#endif
