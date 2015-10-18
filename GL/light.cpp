
#include "light.h"
#include <iostream>

glm::vec4 AbstractLight::getColor(){
  return vColor;
}
void AbstractLight::setColor( glm::vec4 col ){
  this->vColor = col;
}


glm::vec3 DirectionalLight::getDirection(){
  return vDirection;
}
void DirectionalLight::setDirection( glm::vec3 dir ){
  this->vDirection = dir;
}


glm::vec3 PointLight::getPosition(){
  return vPosition;
}
void PointLight::setPosition( glm::vec3 pos ){
  this->vPosition = pos;
}



glm::vec3 SpotLight::getDirection(){
  return vDirection;
}

void SpotLight::setDirection( glm::vec3 dir ){
  this->vDirection = dir;
}

// Return type in radians.
float SpotLight::getHalfAngle(){
  return fHalfAngle;
}

void SpotLight::setHalfAngle( float f ){
  this->fHalfAngle = f;
}

glm::vec4 SpotLight::getConstants(){
  return vConstants;
}

void SpotLight::setConstants( glm::vec4 constants ){
  vConstants = constants;
}

Object* SpotLight::getBoundingGeometry( int lod ){
  Object* o = new Object();
  // Algorithm to determine cutoff distance for the spot light along the direction of influence.
  float fCutoff = this->getColor().a * 0.9 / vConstants.x;

  glm::vec3 vDir = this->getDirection();
  glm::vec3 vDiskCenter = vDir * fCutoff + this->getPosition();

  float fDiskRadius = fCutoff * tan( this->getHalfAngle() );

  std::cout<<"disk-radius:"<<fDiskRadius<<" disk-center:"<<vDiskCenter.x<<","<<vDiskCenter.y<<","<<vDiskCenter.z<<std::endl;fflush(stdout);

  // Procedure to calculate first direction normal. Currently uses +Y axis as up vector. Replace procedure with a less error prone one.
  glm::vec3 vNormal = glm::normalize( glm::cross( vDir, glm::cross( glm::vec3( 0.0f,1.0f,0.0f ), vDir ) ) ) * fDiskRadius;
  std::cout<<" disk-center:"<<vNormal.x<<","<<vNormal.y<<","<<vNormal.z<<std::endl;fflush(stdout);

  for( int i = 0; i < lod; i++ ){
    glm::vec3 vNewNormal = glm::rotate( vNormal, 360.0f / (lod) , vDir );


    // Curved surface estimate.
    o->addSurfaceVN( Vertex( this->getPosition(), glm::vec2( 0, 0 ) ), Vertex( vDiskCenter + vNormal, glm::vec2( 0, 0 ) ) , Vertex( vDiskCenter + vNewNormal, glm::vec2( 0, 0 ) ) );

    // Bottom circle surface estimate.
    o->addSurfaceVN( Vertex( vDiskCenter, glm::vec2( 0, 0 ) ), Vertex( vDiskCenter + vNormal, glm::vec2( 0, 0 ) ) , Vertex( vDiskCenter + vNewNormal, glm::vec2( 0, 0 ) ) );

    std::cout<<"new-normal:"<<vNewNormal.x<<","<<vNewNormal.y<<","<<vNewNormal.z<<std::endl;fflush(stdout);

    vNormal = vNewNormal;
  }
  std::cout<<"Cone finished.\n";fflush( stdout );
  o->setDrawMode( GL_TRIANGLES );
  return o;
}

glm::mat4 SpotLight::getSHMViewMatrix(){
  return glm::lookAt( this->getPosition(), this->getPosition() + this->getDirection(), glm::vec3( 0.0, -1.0, 0.0 ) );
}
