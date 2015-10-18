#include "texture.h"
#include <iostream>




    /*Texture::Texture( ){
      Texture( GL_TEXTURE_2D_MULTISAMPLE );
    }*/

    Texture::Texture( GLenum bindTarget ){
        eInternalFormat = GL_RGBA;
        eFormat = GL_RGBA;
        eType = GL_UNSIGNED_BYTE;
        eBindTarget = bindTarget;
        glInit();

    }

    Texture::Texture( GLenum bindTarget, GLint pTex ){
      glTexBuf = pTex;
      eBindTarget = bindTarget;
    }


    void Texture::glActivate( GLenum texChannel ){
        glActiveTexture( texChannel );
        glBind();
    }

    void Texture::glInit(){
        glGenTextures( 1, &glTexBuf );
    }
    void Texture::setBindTarget( GLenum bindTarget ){
      eBindTarget = bindTarget;
    }
    void Texture::setTextureDataFormat( GLenum internalFormat, GLenum format, GLenum type ){
        eInternalFormat = internalFormat; // Describes internal structure.
        eType = type;// Describes individual component sizes & types. (usually GL_BYTE for 32-bit textures, GL_SHORT for 64-bit textures & GL_FLOAT for 128-bit textures)
        eFormat = format;// Describes component layout. i.e order
    }

    void Texture::glBind(){
        glBindTexture( eBindTarget, glTexBuf );
    }

    GLuint Texture::glGetInternalTexture(){
      return glTexBuf;
    }

  FileTexture::FileTexture( const std::string& filename, const std::string& fileFormat, GLenum internalFormat, GLenum format, GLenum type ):Texture( GL_TEXTURE_2D ){
    setTextureDataFormat( internalFormat, format, type );
    load( filename, fileFormat );
  }
  bool FileTexture::load( const std::string& filename, const std::string& fileFormat ){
    Magick::Blob mBlob;
    Magick::Image* mImg;
    try{
      mImg = new Magick::Image( filename );
      mImg->write( &mBlob, fileFormat );
    }catch( Magick::Error& err ){
      return false;
    }

    //glBindTexture( GL_TEXTURE_2D, glTexBuf );
    Texture::glBind();
    glTexImage2D( GL_TEXTURE_2D, 0, eInternalFormat, mImg->columns(), mImg->rows(), 0, eFormat, eType, mBlob.data() );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  }

  /*void BlankTexture::glBind(){
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, glTexBuf );
  }*/
  void BlankTexture::glGenMipmaps(){
    if( bMipmaps )
      glGenerateMipmap( GL_TEXTURE_2D );
  }
  BlankTexture::BlankTexture( int width, int height, GLenum internalFormat, GLenum format, GLenum type, int iNumSamples, bool mipmaps ):Texture( 0 ){
    if( iNumSamples )
      setBindTarget( GL_TEXTURE_2D_MULTISAMPLE );
    else
      setBindTarget( GL_TEXTURE_2D );
    this->bMipmaps = mipmaps;
    glBind();
    setTextureDataFormat( internalFormat, format, type );

    if( iNumSamples )
      glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, iNumSamples, eInternalFormat, width, height, GL_TRUE );
    else{
      glTexImage2D( GL_TEXTURE_2D, 0, eInternalFormat, width, height, 0, eFormat, eType, NULL );
      if( bMipmaps )
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      else
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if( bMipmaps )
        glGenerateMipmap( GL_TEXTURE_2D );
    }

  }
