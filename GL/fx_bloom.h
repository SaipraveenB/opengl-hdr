#include "technique.h"


class FXBloomH : public AbstractTechnique{
  GLint uTex;
  GLint uRadius;
  int iRadius;
  //float fThreshold;
public:
  FXBloomH( int );
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
};

class FXBloomV : public AbstractTechnique{
  GLint uTex;
  GLint uRadius;
  int iRadius;
  //float fThreshold;
public:
  FXBloomV( int );
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
};
