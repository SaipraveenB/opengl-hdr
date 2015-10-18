#include "scene.h"

Scene::Scene( ){
  pObjects = new std::vector<Object*>;
}

int Scene::addObject( Object* obj ){
  pObjects->push_back( obj );
  return pObjects->size();
}

Object* Scene::getObjectRef( int i ){
  return pObjects->at(i);
}

int Scene::getObjectCount(){
  return pObjects->size();
}
// Transfer vertex attribute arrays into the GL memory
void Scene::glTransfer(){
  for( int i = 0; i < this->getObjectCount(); i++ )
    (this->getObjectRef(i))->glTransfer();

}

// Draw objects along with the required effects. Output is rendered to the currently active Framebuffer(s).
void Scene::glDraw(){
  for( int i = 0; i < this->getObjectCount(); i++ ){
    (this->getObjectRef(i))->glDraw();
  }
}
