#include "fx_directdraw.h"
  FXDirectDrawMS::FXDirectDrawMS( int numSamples ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    iNumSamples = numSamples;
    AbstractTechnique::load("fx/ddms.vs","fx/ddms.fs");
    glLinkUniforms();
  }
  void FXDirectDrawMS::glLinkUniforms(){
    this->glActivate();
    uTex = glGetUniformLocation( shader, "sampler" );
    uNumSamples = glGetUniformLocation( shader, "sampleCount" );
  }

  void FXDirectDrawMS::glTransferUniforms(){
    glUniform1i( uTex, 0 );
    glUniform1i( uNumSamples, iNumSamples );
  }
  void FXDirectDrawMS::glActivate(){
    glUseProgram( shader );
  }

  FXDirectDraw::FXDirectDraw( ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    AbstractTechnique::load("fx/dd.vs","fx/dd.fs");
    glLinkUniforms();
  }
  void FXDirectDraw::glLinkUniforms(){
    this->glActivate();
    uTex = glGetUniformLocation( shader, "sampler" );
  }

  void FXDirectDraw::glTransferUniforms(){
    glUniform1i( uTex, 0 );
  }
  void FXDirectDraw::glActivate(){
    glUseProgram( shader );
  }
