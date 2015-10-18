#include "stdgl.h"
#include "object.h"

class ModelLoader{
public:
  ModelLoader();
  Object* load( const std::string&, bool );
};
