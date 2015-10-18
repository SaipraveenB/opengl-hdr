#include "fx_hdr.h"
FXHDRMS::FXHDRMS( int numSamples ){
  // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
  iNumSamples = numSamples;
  AbstractTechnique::load("fx/hdrms.vs","fx/hdrms.fs");
  glLinkUniforms();
}

void FXHDRMS::setExposure( float f ){
  fExposure = f;
}

void FXHDRMS::glLinkUniforms(){
  this->glActivate();
  uTex = glGetUniformLocation( shader, "sampler" );
  uNumSamples = glGetUniformLocation( shader, "sampleCount" );

  uExposure = glGetUniformLocation( shader, "fExposure" );
}

void FXHDRMS::glTransferUniforms(){
  glUniform1i( uTex, 0 );
  glUniform1i( uNumSamples, iNumSamples );
  glUniform1f( uExposure, fExposure );
}

void FXHDRMS::glActivate(){
  glUseProgram( shader );
}

FXHDR::FXHDR(){
  // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
  AbstractTechnique::load("fx/hdr.vs","fx/hdr.fs");
  glLinkUniforms();
}

void FXHDR::setExposure( float f ){
  fExposure = f;
}

void FXHDR::glLinkUniforms(){
  this->glActivate();
  uTex = glGetUniformLocation( shader, "sampler" );

  uExposure = glGetUniformLocation( shader, "fExposure" );

}

void FXHDR::glTransferUniforms(){
  glUniform1i( uTex, 0 );
  glUniform1f( uExposure, fExposure );
}
void FXHDR::glActivate(){
  glUseProgram( shader );
}
