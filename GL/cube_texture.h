#ifndef OGLDEV_CUBEMAP_TEXTURE_H
#define	OGLDEV_CUBEMAP_TEXTURE_H

#include "stdgl.h"
#include <Magick++.h>
#include <iostream>
#include "texture.h"

using namespace std;

class CubemapTexture : public Texture
{
public:

  CubemapTexture(const string& Directory,
  const string& PosXFilename,
  const string& NegXFilename,
  const string& PosYFilename,
  const string& NegYFilename,
  const string& PosZFilename,
  const string& NegZFilename);

  ~CubemapTexture();

  bool load();

private:

  string m_fileNames[6];
  GLuint m_textureObj;
};


class BlankCubemapTexture : public Texture
{
public:

  BlankCubemapTexture(int width, int height, GLenum internalFormat, GLenum format, GLenum type);

  ~BlankCubemapTexture();

  bool load();

private:
  int width;
  int height;
  GLuint m_textureObj;
};


#endif
