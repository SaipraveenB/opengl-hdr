#include "stdgl.h"
#include "render_target.h"
#include <iostream>

  RenderTarget::RenderTarget( int width, int height, int numSamples, bool mipmaps ){
    RenderTarget( width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, numSamples, mipmaps );
  }
  RenderTarget::RenderTarget( int width, int height, GLenum internalFormat, GLenum format, GLenum type, int numSamples, bool mipmaps ){
    pTex = new BlankTexture( width, height, internalFormat, format, type, numSamples, mipmaps );
    pDepth = new BlankTexture( width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, numSamples, false );
    this->width = width;
    this->height = height;
    this->numSamples = numSamples;
    this->mipmaps = mipmaps;
    glGenFramebuffers( 1, &fbo );
    glInit();
  }
  Texture* RenderTarget::getColorTexture(){
    return pTex;
  }
  Texture* RenderTarget::getDepthTexture(){
    return pDepth;
  }
  void RenderTarget::generateMipmaps(){
    if( this->mipmaps ){
      pTex->glBind();
      pTex->glGenMipmaps();
    }
  }
  void RenderTarget::glInit(){
    this->glBind();
    pTex->glBind();
    pDepth->glBind();

    if( numSamples ){
      glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pTex->glGetInternalTexture() ,0 );
      glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pDepth->glGetInternalTexture() ,0 );
    }else{
      glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTex->glGetInternalTexture(), 0 );
      glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pDepth->glGetInternalTexture(), 0 );
    }
  }
  void RenderTarget::glBind(){
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    glViewport( 0, 0, width, height );// Set pipeline output to produce data at the right resolution.
  }

  void RenderTarget::glBind( GLenum target ){
    glBindFramebuffer( target, fbo );
    //glViewport( 0, 0, width, height );// Set pipeline output to produce data at the right resolution.
  }


  CubemapRenderTarget::CubemapRenderTarget( int width, int height ){
    CubemapRenderTarget( width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE );
  }
  CubemapRenderTarget::CubemapRenderTarget( int width, int height, GLenum internalFormat, GLenum format, GLenum type ){
    pTex = new BlankCubemapTexture( width, height, internalFormat, format, type );
    pDepth = new BlankCubemapTexture( width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT );
    this->width = width;
    this->height = height;
    glGenFramebuffers( 1, &fbo );
    glInit();
  }
  Texture* CubemapRenderTarget::getColorTexture(){
    return pTex;
  }

  Texture* CubemapRenderTarget::getDepthTexture(){
    return pDepth;
  }

  void CubemapRenderTarget::glInit(){
    this->glBind();
    pTex->glBind();
    pDepth->glBind();

  }
  void CubemapRenderTarget::glChangeFace( GLenum face ){
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, pTex->glGetInternalTexture(), 0 );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, pDepth->glGetInternalTexture(), 0 );
  }

  void CubemapRenderTarget::glBind( GLenum face ){
    this->glChangeFace( face );
    glViewport( 0, 0, width, height );// Set pipeline output to produce data at the right resolution.
  }

  void CubemapRenderTarget::glBind( ){
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    glViewport( 0, 0, width, height );// Set pipeline output to produce data at the right resolution.
  }
