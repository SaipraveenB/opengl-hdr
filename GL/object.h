

#include "stdgl.h"
#include <vector>
#include "texture.h"

#ifndef OBJECT_H
#define OBJECT_H
struct Vertex{
    glm::vec3 pos;
    glm::vec2 texColor;
    glm::vec3 normal;

    Vertex():pos( glm::vec3(0,0,0) ),texColor( glm::vec2(0,0) ),normal( glm::vec3(0,0,0) ){

    }

    Vertex( glm::vec3 pos, glm::vec2 texColor, glm::vec3 normal ):pos(pos),texColor(texColor),normal(normal){
    }

    Vertex( glm::vec3 pos, glm::vec2 texColor ):pos(pos),texColor(texColor),normal( glm::vec3(0,0,0) ){

    }
};

class Object{
private:
    // Bulk data structures.
    std::vector<Vertex>* vBuf;
    std::vector<int>* iBuf;
    std::vector<Texture*>* pTexs;

    // GPU memory addresses.
    GLuint glVBuf;
    GLuint glIBuf;
    GLuint glArrayDef;

    // Draw properties.
    GLenum glDrawMode;

    const int MAX_TEXTURES = 8;

public:
    Object();

    int addVertex( glm::vec3 pos, glm::vec2 texColor );
    int addVertex( Vertex v );
    int addVertex( glm::vec3 pos, glm::vec2 texColor, glm::vec3 normal );

    int addSurfaceI( int a, int b, int c );
    int addSurfaceV( Vertex a, Vertex b, Vertex c );
    int addSurfaceVN( Vertex a, Vertex b, Vertex c );

    int getIndexCount();

    void glSetTexture( int i, const std::string& filename );
    void glSetTexture( int i, Texture* pTex );
    void setDrawMode( GLenum drawMode );

    void glTransfer();

    void glBind();
    void glDraw();
};

class ScreenQuad : public Object{
public:
  ScreenQuad();
};
#endif
