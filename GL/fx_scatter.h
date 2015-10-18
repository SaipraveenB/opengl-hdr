
#include "technique.h"
#include "light.h"

class FXScatter : public WVPBasedTechnique{
  GLint uCubeTex;
  GLint uLightPos;
  GLint uLightConstants;
  GLint uSampleDensity;
  GLint uLightDir;
  GLint uWVPSHM;
  GLint uViewI;


  float fSampleDensity;
  SpotLight* pLight;
  glm::mat4 mWVPSHM;

public:
  FXScatter();
  void glActivate();
  void glTransferUniforms();
  void setLight( SpotLight* );
  void setSampleDensity( float );
  void glLinkUniforms();
  void setWVPSHM( glm::mat4 );
};
