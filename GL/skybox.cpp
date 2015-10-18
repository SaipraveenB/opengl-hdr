#include "skybox.h"

Skybox::Skybox(){
  this->pCubeTex = NULL;
  this->pObj = new ScreenQuad();

  AbstractTechnique::load("fx/skybox.vs","fx/skybox.fs");
  this->glActivate();
  this->glLinkUniforms();
}

void Skybox::setTexture( const string& Directory,
const string& PosXFilename,
const string& NegXFilename,
const string& PosYFilename,
const string& NegYFilename,
const string& PosZFilename,
const string& NegZFilename ){

  this->pCubeTex = new CubemapTexture( Directory, PosXFilename, NegXFilename, PosYFilename, NegYFilename, PosZFilename, NegZFilename );

}

void Skybox::setTexture( Texture* tex ){
  this->pCubeTex = tex;
}

Texture* Skybox::getTexture(){
  return this->pCubeTex;
}

void Skybox::glDraw(){
  this->glActivate();
  this->glTransferUniforms();
  this->pCubeTex->glActivate( GL_TEXTURE0 );
  this->pObj->glDraw();

}

void Skybox::glActivate(){
  glUseProgram( shader );
}

void Skybox::glTransfer(){

  this->pObj->glTransfer();

}

void Skybox::glLinkUniforms(){

  this->glActivate();
  WVPBasedTechnique::glLinkUniforms();

  this->uCubeTex = glGetUniformLocation( shader, "sampler0" );
  this->uWVPi = glGetUniformLocation( shader, "mWVPi" );
  if( this->uCubeTex!= -1 ){
    glUniform1i( uCubeTex, 0 );
  }

}

void Skybox::glTransferUniforms(){

  glm::mat4 matWVPi = glm::inverse( WVPBasedTechnique::getWVP() );

  if( uWVPi != -1 ){
    glUniformMatrix4fv( uWVPi, 1, GL_FALSE, glm::value_ptr(matWVPi) );
  }

  if( this->uCubeTex!= -1 ){
    glUniform1i( uCubeTex, 0 );
  }
}
