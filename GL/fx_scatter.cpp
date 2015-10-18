#include "fx_scatter.h"
/*
  Meant to be used with a spot light.
  Enable this technique and draw the spot lights bounding cone.
*/

FXScatter::FXScatter(){
  AbstractTechnique::load("fx/scatter.vs","fx/scatter.fs");
  this->glLinkUniforms();
}

void FXScatter::glLinkUniforms(){
  this->WVPBasedTechnique::glLinkUniforms();

  this->uCubeTex = glGetUniformLocation( shader, "shadow" );
  this->uLightPos = glGetUniformLocation( shader, "lightPos" );
  this->uLightDir = glGetUniformLocation( shader, "lightDir" );
  this->uLightConstants = glGetUniformLocation( shader, "lightConstants" );
  this->uSampleDensity = glGetUniformLocation( shader, "fSampleDensity" );
  this->uWVPSHM = glGetUniformLocation( shader, "mWVPshm" );
  this->uViewI = glGetUniformLocation( shader, "mViewI" );
}

void FXScatter::glActivate(){
  glUseProgram( this->shader );
}

void FXScatter::setSampleDensity( float f ){
  fSampleDensity = f;
}

void FXScatter::setWVPSHM( glm::mat4 m ){
  this->mWVPSHM = m;
}

void FXScatter::glTransferUniforms(){
  this->WVPBasedTechnique::glTransferUniforms();
  glUniform1i( uCubeTex, 0 );
  glm::vec3 pos = glm::vec3( this->getView() * this->getWorld() * glm::vec4( pLight->getPosition(), 1.0f ) );
  glm::vec4 constants = pLight->getConstants();
  glm::vec3 dir = glm::vec3( this->getView() * this->getWorld() * glm::vec4( pLight->getDirection(), 0.0f ) );
  glUniform3fv( uLightPos, 1, (const float*)&pos );
  glUniform3fv( uLightDir, 1, (const float*)&dir );
  glUniform4fv( uLightConstants, 1, (const float*)&constants );
  glUniform1f( uSampleDensity, fSampleDensity );
  glUniformMatrix4fv( uWVPSHM, 1, GL_FALSE, glm::value_ptr(mWVPSHM) );
  glUniformMatrix4fv( uViewI, 1, GL_FALSE, glm::value_ptr( glm::inverse( this->getView() ) ) );

}

void FXScatter::setLight( SpotLight* l ){
  pLight = l;
}
