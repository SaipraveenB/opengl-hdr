#include "stdgl.h"
#include "object.h"

#ifndef LIGHT_H
#define LIGHT_H
class AbstractLight{
private:
  glm::vec4 vColor;

public:
  glm::vec4 getColor();
  void setColor( glm::vec4 );

};

class AmbientLight : public AbstractLight{

};

class DirectionalLight : public AbstractLight{
protected:
  glm::vec3 vDirection;
public:
  glm::vec3 getDirection();
  void setDirection( glm::vec3 );
};


class PointLight : public AbstractLight{
protected:
  glm::vec3 vPosition;
public:
  glm::vec3 getPosition();
  void setPosition( glm::vec3 );
};

class SpotLight : public PointLight{
private:
  glm::vec3 vDirection;
  float fHalfAngle;
  float fFalloff;
  glm::vec4 vConstants;
public:
  glm::vec3 getDirection();
  void setDirection( glm::vec3 );
  float getHalfAngle();
  void setHalfAngle( float );
  glm::vec4 getConstants();
  void setConstants( glm::vec4 );
  Object* getBoundingGeometry( int );
  glm::mat4 getSHMViewMatrix();
};
#endif
