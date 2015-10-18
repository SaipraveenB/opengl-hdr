#include "stdgl.h"
#include "texture.h"
#include "cube_texture.h"


#ifndef RENDERTARGET_H
#define RENDERTARGET_H

class AbstractRenderTarget{
protected:
  GLuint fbo;
public:
  virtual void glInit() = 0;
};

class CubemapRenderTarget : public AbstractRenderTarget{
  BlankCubemapTexture* pTex;
  BlankCubemapTexture* pDepth;


  int width;
  int height;

public:
  CubemapRenderTarget( int width, int height );
  CubemapRenderTarget( int width, int height, GLenum internalFormat, GLenum format, GLenum type );
  void glBind( GLenum );
  void glBind();
  void glInit();
  void glChangeFace( GLenum );
  Texture* getColorTexture();
  Texture* getDepthTexture();
};

class RenderTarget{
  GLuint fbo;
  BlankTexture* pTex;
  BlankTexture* pDepth;

  int width;
  int height;
  int numSamples;
  bool mipmaps;
public:
  RenderTarget( int width, int height, int, bool );
  RenderTarget( int width, int height, GLenum internalFormat, GLenum format, GLenum type, int, bool );
  void glBind();
  void glBind( GLenum target );
  void glInit();
  void generateMipmaps();
  Texture* getColorTexture();
  Texture* getDepthTexture();
};
#endif
