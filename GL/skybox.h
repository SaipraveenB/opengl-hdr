#include "stdgl.h"
#include "cube_texture.h"
#include "technique.h"
#include "object.h"

class Skybox : public WVPBasedTechnique, public GeometricEffect{
private:
  Texture* pCubeTex;
  //SkyboxTechnique* pTech;
  ScreenQuad* pObj;

  GLint uCubeTex;
  GLint uWVPi;
public:
  Skybox();
  void setTexture( const string& Directory,const string& PosXFilename,const string& NegXFilename,const string& PosYFilename,const string& NegYFilename,const string& PosZFilename,const string& NegZFilename );
  void setTexture( Texture* );
  void glDraw();
  void glTransfer();
  void glTransferUniforms();
  void glLinkUniforms();
  void glActivate();
  Texture* getTexture();
};
