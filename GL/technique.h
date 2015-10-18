#include "stdgl.h"
#include "object.h"
#include "render_target.h"
#include "texture.h"
#include <iostream>

#ifndef TECHNIQUE_H
#define TECHNIQUE_H
class AbstractTechnique{

private:
  std::string vsFile;
  std::string fsFile;
  void loadShader( GLuint obj, const char* filename );
protected:

  GLuint shader;
  virtual void glLinkUniforms() = 0;

public:
  AbstractTechnique();
  virtual ~AbstractTechnique() = 0;
  virtual void glActivate();
  virtual void glTransferUniforms();
  void load( const std::string& vs, const std::string& fs );
  void glPass( Texture*, RenderTarget*, ScreenQuad* );
};

class WVPBasedTechnique : virtual public AbstractTechnique{

protected:
  glm::mat4 matWorld;
  glm::mat4 matView;
  glm::mat4 matProj;

  GLint mWorld;
  GLint mView;
  GLint mProj;
  GLint mWVP;


public:
  //WVPBasedTechnique();
  //virtual ~WVPBasedTechnique();
  void setWorld( glm::mat4 );
  void setView( glm::mat4 );
  void setProj( glm::mat4 );

  glm::mat4 getWorld();
  glm::mat4 getView();
  glm::mat4 getProj();
  glm::mat4 getWVP();


  void glTransferUniforms();
  void glLinkUniforms();
};

class GeometricEffect : virtual public AbstractTechnique{

public:
  virtual void glDraw() = 0;
  virtual void glTransfer() = 0;

};
#endif
