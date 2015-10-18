#include "technique.h"


class FXHDRMS : public AbstractTechnique{
  GLint uTex;
  GLint uNumSamples;
  GLint uExposure;

  float fExposure;
  int iNumSamples;
public:
  FXHDRMS( int );
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
  void setExposure( float );
};

class FXHDR : public AbstractTechnique{
  GLint uTex;
  GLint uNumSamples;
  GLint uExposure;

  float fExposure;
public:
  FXHDR();
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
  void setExposure( float );
};
