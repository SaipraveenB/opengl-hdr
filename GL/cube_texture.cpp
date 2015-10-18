#include <iostream>
#include "stdgl.h"
#include "cube_texture.h"

static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

  CubemapTexture::CubemapTexture(const string& Directory,
  const string& PosXFilename,
  const string& NegXFilename,
  const string& PosYFilename,
  const string& NegYFilename,
  const string& PosZFilename,
  const string& NegZFilename) : Texture( GL_TEXTURE_CUBE_MAP )
{
  string::const_iterator it = Directory.end();
  it--;
  string BaseDir = (*it == '/') ? Directory : Directory + "/";

  m_fileNames[0] = BaseDir + PosXFilename;
  m_fileNames[1] = BaseDir + NegXFilename;
  m_fileNames[2] = BaseDir + PosYFilename;
  m_fileNames[3] = BaseDir + NegYFilename;
  m_fileNames[4] = BaseDir + PosZFilename;
  m_fileNames[5] = BaseDir + NegZFilename;

  glTexBuf = 0;
  load();
}

CubemapTexture::~CubemapTexture()
{
  if (glTexBuf != 0) {
    glDeleteTextures(1, &glTexBuf);
  }
}

bool CubemapTexture::load()
{
  //glGenTextures(1, &m_textureObj);
  //glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
  Texture::glBind();

  Magick::Image* pImage = NULL;
  Magick::Blob blob;

  for (unsigned int i = 0 ; i < 6 ; i++) {
    pImage = new Magick::Image(m_fileNames[i]);

    try {
      pImage->write(&blob, "RGBA");
    }
    catch (Magick::Error& Error) {
      cout << "Error loading texture '" << m_fileNames[i] << "': " << Error.what() << endl;
      delete pImage;
      return false;
    }

    glTexImage2D(types[i], 0, GL_RGB, pImage->columns(), pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());

    delete pImage;
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return true;
}


BlankCubemapTexture::BlankCubemapTexture( int width, int height, GLenum internalFormat, GLenum format, GLenum type ) : Texture( GL_TEXTURE_CUBE_MAP )
{
  this->width = width;
  this->height = height;
  Texture::setTextureDataFormat( internalFormat, format, type );
  m_textureObj = 0;
  load();
}

BlankCubemapTexture::~BlankCubemapTexture()
{
  if (m_textureObj != 0) {
    glDeleteTextures(1, &m_textureObj);
  }
}

bool BlankCubemapTexture::load()
{
  //glGenTextures(1, &m_textureObj);
  //glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
  Texture::glBind();

  for (unsigned int i = 0 ; i < 6 ; i++) {
    glTexImage2D(types[i], 0, eInternalFormat, width, height, 0, eFormat, eType, NULL);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return true;
}


/*void CubemapTexture::glActivate(GLenum TextureUnit)
{
  glActiveTexture(TextureUnit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
}*/
