#include "technique.h"


class FXCombine : public AbstractTechnique{
  GLint uTex0;
  GLint uTex1;

public:
  FXCombine( );
  void glLinkUniforms();
  void glTransferUniforms();
  void glActivate();
};
