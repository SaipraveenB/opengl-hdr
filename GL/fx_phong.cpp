#include "fx_phong.h"
#include <iostream>
#include <stdio.h>

FXPhong::FXPhong(){
  AbstractTechnique::load("fx/main.vs","fx/main.fs");
  vPL = new std::vector<PointLight*>;
  vDL = new std::vector<DirectionalLight*>;
  this->glLinkUniforms();
}

void FXPhong::glLinkUniforms(){
  this->glActivate();
  WVPBasedTechnique::glLinkUniforms();

  // Pre-multiply the three matrices as for standard transformation we need the WVP but keep the three individual matrices as well.
  // This is to prevent the matrices getting multiplied repeatedly for every pixel.

  tColor = glGetUniformLocation( shader, "sampler0" );
  tEnv = glGetUniformLocation( shader, "env" );
  tShadow = glGetUniformLocation( shader, "shadow" );

  uLightPos = glGetUniformLocation( shader, "lightPos" );
  uLightDir = glGetUniformLocation( shader, "lightDir" );
  uLightPNum = glGetUniformLocation( shader, "numPLights" );
  uLightDNum = glGetUniformLocation( shader, "numDLights" );
  uLightPColor = glGetUniformLocation( shader, "lightPColor" );
  uLightDColor = glGetUniformLocation( shader, "lightDColor" );

  uNoise = glGetUniformLocation( shader, "noise" );

  uViewI = glGetUniformLocation( shader, "mViewI" );

  uSpecularExponent = glGetUniformLocation( shader, "fSpecularExponent" );
  uIntensityPackingConstant = glGetUniformLocation( shader, "fIntensityPackingConstant" );
  uAmbient = glGetUniformLocation( shader, "fAmbient" );

}
void FXPhong::setIntensityPackingConstant( float f ){
  fIntensityPackingConstant = f;
}

void FXPhong::glTransferUniforms(){
  this->glActivate();
  this->WVPBasedTechnique::glTransferUniforms();
  //std::cout<<"Transferred WVP\n";fflush(stdout);
  std::vector<glm::vec3> lightPos;
  std::vector<glm::vec3> lightDir;
  std::vector<glm::vec4> lightPColor;
  std::vector<glm::vec4> lightDColor;

  int lightPNum = vPL->size();
  int lightDNum = vDL->size();

  for( int i = 0; i < vPL->size(); i ++ )
    lightPos.push_back( glm::vec3( WVPBasedTechnique::getWorld() * WVPBasedTechnique::getView() * glm::vec4( vPL->at(i)->getPosition().x, vPL->at(i)->getPosition().y, vPL->at(i)->getPosition().z, 1.0f ) ) );

  for( int i = 0; i < vDL->size(); i ++ )
    lightDir.push_back( glm::vec3( WVPBasedTechnique::getWorld() * WVPBasedTechnique::getView() * glm::vec4( vDL->at(i)->getDirection().x, vDL->at(i)->getDirection().y, vDL->at(i)->getDirection().z, 0.0f ) ) );

  for( int i = 0; i < vPL->size(); i ++ )
    lightPColor.push_back( vPL->at(i)->getColor() );

  for( int i = 0; i < vDL->size(); i ++ )
    lightDColor.push_back( vDL->at(i)->getColor() );

  if( uLightPos!= -1 ){
    glUniform3fv( uLightPos, lightPNum, (const float*)lightPos.data() );
  }

  if( uLightDir!= -1 ){
    glUniform3fv( uLightDir, lightDNum, (const float*)lightDir.data() );
  }

  if( uLightPColor!= -1 ){
    glUniform4fv( uLightPColor, lightPNum, (const float*)lightPColor.data() );
  }

  if( uLightDColor!= -1 ){
    glUniform4fv( uLightDColor, lightDNum, (const float*)lightDColor.data() );
  }

  if( uLightPNum!= -1 ){
    glUniform1i( uLightPNum, lightPNum );
  }

  if( uLightDNum!= -1 ){
    glUniform1i( uLightDNum, lightDNum );
  }

  if( uAmbient != -1 ){
    glUniform1f( uAmbient, pAL->getColor().w );
  }

  if( uSpecularExponent != -1 ){
    glUniform1f( uSpecularExponent, fSpecularExponent );
  }

  if( uIntensityPackingConstant != -1 ){
    glUniform1f( uIntensityPackingConstant, fIntensityPackingConstant );
  }

  if( uViewI != -1 ){
    glUniformMatrix4fv( uViewI, 1, GL_FALSE, glm::value_ptr( glm::inverse( WVPBasedTechnique::getView() ) ) );
  }

  if( tColor!= -1 ){
    glUniform1i( tColor, 0 );
  }

  if( tEnv!= -1 ){
    glUniform1i( tEnv, 1 );
  }

  if( tShadow!= -1 ){
    glUniform1i( tShadow, 3 );
  }

  if( uNoise!= -1 ){
    glUniform1i( uNoise, 4 );
  }

  //std::cout<<"Finished light transfer.\n";fflush(stdout);
}

void FXPhong::addLightP(PointLight* l){
  vPL->push_back(l);
}
void FXPhong::addLightD(DirectionalLight* l){
  vDL->push_back(l);
}
void FXPhong::setLightA(AmbientLight* l){
  this->pAL = l;
}

void FXPhong::setSpecularExponent( float f ){
  fSpecularExponent = f;
}

void FXPhong::glActivate(){
  glUseProgram( shader );
}
