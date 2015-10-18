#include "noise.h"
#include <math.h>


GradientCube::GradientCube( int l, int b, int h ){
    this->length = l;
    this->height = h;
    this->breadth = b;
    pCube = new glm::vec3[ l * b * h ];

    //std::default_random_engine generator;
    //std::uniform_real_distribution<double> distribution(0.0,1.0);

    for( int i = 0; i < l*b*h; i++ ){
      float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      pCube[i] = glm::normalize( glm::vec3( x, y, z ) );
    }

    glGenTextures( 1, &pNLT );
    glBindTexture( GL_TEXTURE_2D, pNLT );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, length, breadth * height, 0, GL_RGB, GL_FLOAT, getData() );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}
int GradientCube::getBreadth(){ return breadth; }
int GradientCube::getHeight(){ return height; }
int GradientCube::getLength(){ return length; }
float* GradientCube::getData(){ return (float*)pCube; }
GLuint GradientCube::getTexture(){ return pNLT; }
