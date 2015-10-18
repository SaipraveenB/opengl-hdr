
#include "technique.h"

class FXProject : public WVPBasedTechnique{
  GLint uCubeTex;
  GLint uLightPos;

  glm::vec3 vLightPos;

public:
  FXProject();
  void glActivate();
  void glTransferUniforms();
  
};
