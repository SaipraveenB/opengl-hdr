#include "technique.h"
#include <vector>
#include "light.h"
#include "noise.h"

class FXPhong : public WVPBasedTechnique{

  std::vector<PointLight*>* vPL;
  std::vector<DirectionalLight*>* vDL;
  AmbientLight* pAL;

  glm::mat4 matWorld;
  glm::mat4 matView;
  //glm::mat4 matViewI;
  glm::mat4 matProj;
  float fSpecularExponent;
  float fIntensityPackingConstant;

  GLint tColor;
  GLint tEnv;
  GLint tShadow;

  GLint uAmbient;
  GLint vLight;

  GLint uLightPos;
  GLint uLightDir;
  GLint uLightPNum;
  GLint uLightDNum;
  GLint uLightPColor;
  GLint uLightDColor;
  GLint uSpecularExponent;
  GLint uIntensityPackingConstant;
  GLint uViewI;
  GLint uNoise;

  GradientCube* pCube;

public:
  FXPhong();

  void glLinkUniforms();
  void glActivate();
  void glTransferUniforms();

  void addLightP(PointLight* l);
  void addLightD(DirectionalLight* l);
  void setLightA(AmbientLight* l);
  void setSpecularExponent( float );
  void setIntensityPackingConstant( float );
};
