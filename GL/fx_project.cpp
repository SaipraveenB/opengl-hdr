#include "fx_project.h"
FXProject::FXProject(){
  AbstractTechnique::load("fx/std.vs","fx/blank.fs");
  this->WVPBasedTechnique::glLinkUniforms();
}
void FXProject::glActivate(){
  glUseProgram( this->shader );
}
void FXProject::glTransferUniforms(){
  this->WVPBasedTechnique::glTransferUniforms();
}
