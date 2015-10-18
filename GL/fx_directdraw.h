#include "technique.h"


class FXDirectDrawMS : public AbstractTechnique{
  GLint uTex;
  GLint uNumSamples;

  int iNumSamples;
public:
  FXDirectDrawMS( int );
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
};

class FXDirectDraw : public AbstractTechnique{
  GLint uTex;
  GLint uNumSamples;

public:
  FXDirectDraw();
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
};
