#include "fx_bloom.h"
FXBloomH::FXBloomH( int i ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    iRadius = i;
    AbstractTechnique::load("fx/hdr.vs","fx/bloom_h.fs");
    glLinkUniforms();
  }
  void FXBloomH::glLinkUniforms(){
    this->glActivate();
    uTex = glGetUniformLocation( shader, "sampler" );
    uRadius = glGetUniformLocation( shader, "iRadius" );
  }

  void FXBloomH::glTransferUniforms(){
    glUniform1i( uTex, 0 );
    glUniform1i( uRadius, iRadius );
  }
  void FXBloomH::glActivate(){
    glUseProgram( shader );
  }

  FXBloomV::FXBloomV( int i ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    iRadius = i;
    AbstractTechnique::load("fx/hdr.vs","fx/bloom_v.fs");
    glLinkUniforms();
  }
  void FXBloomV::glLinkUniforms(){
    this->glActivate();
    uTex = glGetUniformLocation( shader, "sampler" );
    uRadius = glGetUniformLocation( shader, "iRadius" );
  }

  void FXBloomV::glTransferUniforms(){
    glUniform1i( uTex, 0 );
    glUniform1i( uRadius, iRadius );
  }
  void FXBloomV::glActivate(){
    glUseProgram( shader );
  }
