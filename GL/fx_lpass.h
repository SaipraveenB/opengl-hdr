#include "technique.h"


class FXLightPass : public AbstractTechnique{
  GLint uTex;
  GLint uThreshold;

  float fThreshold;
public:
  FXLightPass( float );
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
};
