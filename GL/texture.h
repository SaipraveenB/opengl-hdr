
#include "stdgl.h"
#include <Magick++.h>

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H
class Texture{
  protected:
    GLuint glTexBuf;
    GLenum eInternalFormat;
    GLenum eFormat;
    GLenum eType;
    GLenum eBindTarget;
  public:

    Texture( GLenum bindTarget );
    Texture( GLenum bindTarget, GLint pTex );
    Texture();
    void setTextureDataFormat( GLenum internalFormat, GLenum format, GLenum type );
    void setBindTarget( GLenum bindTarget );
    void glActivate( GLenum texChannel );
    void glInit();
    void glBind();
    GLuint glGetInternalTexture();

};

class FileTexture : public Texture{

public:
  FileTexture( const std::string& filename, const std::string& fileFormat, GLenum internalFormat, GLenum format, GLenum type );
  bool load( const std::string& filename, const std::string& fileFormat );
};

class BlankTexture : public Texture{
  int iNumSamples;
  bool bMipmaps;
public:
  BlankTexture( int, int, GLenum, GLenum, GLenum, int, bool );
  void glGenMipmaps();
  //void glBind();
};
#endif
