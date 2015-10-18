
#include "technique.h"

class FXShadow : public WVPBasedTechnique{
  GLint uCubeTex;
  GLint uLightPos;

  glm::vec3 vLightPos;

public:
  FXShadow();
  void glActivate();
  void glTransferUniforms();
  void setLightPos( glm::vec3 );
  void glLinkUniforms();
};
