#include "fx_combine.h"
FXCombine::FXCombine( ){
    // Compulsory for every FX technique... load the necessary shaders.. The state will otherwise be undefined.
    AbstractTechnique::load("fx/hdr.vs","fx/combine.fs");
    glLinkUniforms();
  }
  void FXCombine::glLinkUniforms(){
    this->glActivate();
    uTex0 = glGetUniformLocation( shader, "sampler0" );
    uTex1 = glGetUniformLocation( shader, "sampler1" );
  }

  void FXCombine::glTransferUniforms(){
    glUniform1i( uTex0, 0 );
    glUniform1i( uTex1, 1 );
  }
  void FXCombine::glActivate(){
    glUseProgram( shader );
  }
