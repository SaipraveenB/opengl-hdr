#include "fx_lpass.h"
FXLightPass::FXLightPass( float f ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    fThreshold = f;
    AbstractTechnique::load("fx/hdr.vs","fx/lpass.fs");
    glLinkUniforms();
  }
  void FXLightPass::glLinkUniforms(){
    this->glActivate();
    uTex = glGetUniformLocation( shader, "sampler" );
    uThreshold = glGetUniformLocation( shader, "fThreshold" );
  }

  void FXLightPass::glTransferUniforms(){
    glUniform1i( uTex, 0 );
    glUniform1f( uThreshold, fThreshold );
  }
  void FXLightPass::glActivate(){
    glUseProgram( shader );
  }
