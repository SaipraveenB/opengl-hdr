#include "fx_shadow.h"
FXShadow::FXShadow(){
  AbstractTechnique::load("fx/shadow.vs","fx/shadow.fs");
  this->glLinkUniforms();
}
void FXShadow::glLinkUniforms(){
  this->WVPBasedTechnique::glLinkUniforms();

  this->uCubeTex = glGetUniformLocation( shader, "shadow" );
  this->uLightPos = glGetUniformLocation( shader, "lightPos" );
}
void FXShadow::glActivate(){
  glUseProgram( this->shader );
}
void FXShadow::glTransferUniforms(){
  this->WVPBasedTechnique::glTransferUniforms();
  glUniform1i( uCubeTex, 2 );
  glUniform3fv( uLightPos, 1, (const float*)&vLightPos );
}


void FXShadow::setLightPos( glm::vec3 pos ){
  vLightPos = pos;
}
