#include <vector>
#include <iostream>
#include "stdgl.h"

class GradientCube{
protected:
  int breadth;
  int length;
  int height;

  glm::vec3* pCube;
  GLuint pNLT;
public:
  GradientCube( int, int, int );
  int getBreadth();
  int getHeight();
  int getLength();
  float* getData();
  GLuint getTexture();
};
