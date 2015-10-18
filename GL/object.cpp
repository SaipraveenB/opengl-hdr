#include "object.h"

#include <iostream>


using namespace std;

Object::Object(){
    vBuf = new vector<Vertex>;
    iBuf = new vector<int>;
    pTexs = new vector<Texture*>;

    for( int i =0; i < MAX_TEXTURES; i++ )
      pTexs->push_back(NULL);

}

int Object::addVertex( glm::vec3 pos, glm::vec2 texColor ){
    addVertex( pos, texColor, glm::vec3(0.0,0.0,0.0) );
}

int Object::addVertex( glm::vec3 pos, glm::vec2 texColor, glm::vec3 normal ){
    Vertex v;
    v.pos = pos;
    v.texColor = texColor;
    v.normal = normal;
    vBuf->push_back( v );
    return vBuf->size()-1;
}

int Object::addVertex( Vertex v ){
  vBuf->push_back( v );
  return vBuf->size()-1;
}

// This function also autocomputes normal for this surface and aggregates with the three vertices. Designed for Goraud-shading.

void Object::setDrawMode( GLenum drawMode ){
  this->glDrawMode = drawMode;
}


glm::vec3 computeNormal( glm::vec3 va, glm::vec3 vb, glm::vec3 vc ){
    glm::vec3 dab = glm::vec3( va - vb );
    glm::vec3 dbc = glm::vec3( vb - vc );

    glm::vec3 normal = glm::normalize( glm::cross( dab, dbc ) );
}

int Object::addSurfaceV( Vertex a, Vertex b, Vertex c ){
  return addSurfaceI( addVertex(a), addVertex(b), addVertex(c) );
}

int Object::addSurfaceVN( Vertex a, Vertex b, Vertex c ){
  glm::vec3 normal = glm::cross((a.pos - b.pos),(b.pos - c.pos));
  a.normal = b.normal = c.normal = normal;
  return addSurfaceV( a, b, c );
}

int Object::addSurfaceI( int a, int b, int c ){
    iBuf->push_back( a );
    iBuf->push_back( b );
    iBuf->push_back( c );

    /*glm::vec3 normal = computeNormal( vBuf->at(a), vBuf->at(b), vBuf->at(c) );

    vBuf->at(a).normal += normal;
    vBuf->at(b).normal += normal;
    vBuf->at(c).normal += normal;*/

    return iBuf->size()/3 - 1;
}

int Object::getIndexCount(){
  return iBuf->size();
}

void Object::glSetTexture( int i, const std::string& filename ){
  pTexs->at(i) = new FileTexture( filename, "RGBA", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE );
  //(pTexs->at(i))->glInit();
  //(pTexs->at(i))->load( filename );
}

void Object::glSetTexture( int i, Texture* pTex ){
  pTexs->at(i) = pTex;
}

void Object::glTransfer(){

    glGenBuffers( 1, &glVBuf );
    glGenBuffers( 1, &glIBuf );
    glGenVertexArrays( 1, &glArrayDef );

    glBindVertexArray( glArrayDef );
    // Bind the vertex & index buffers.
    glBindBuffer( GL_ARRAY_BUFFER, glVBuf );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIBuf );
    // Send data to bound buffers.
    glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex) * vBuf->size(), vBuf->data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * iBuf->size(), iBuf->data(), GL_STATIC_DRAW );

    // Bind the array definition.
    // Set array attributes.
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( const GLvoid*)0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( const GLvoid* )12 );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), ( const GLvoid* )20 );


}

// Setup state for draw.
void Object::glBind(){
    glBindVertexArray( glArrayDef );
    glBindBuffer( GL_ARRAY_BUFFER, glVBuf );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glIBuf );
}

void Object::glDraw(){
  this->glBind();
  for( int i = 0; i < MAX_TEXTURES; i++ ){
    if( pTexs->at(i) != NULL )
      (pTexs->at(i))->glActivate( GL_TEXTURE0 + i );
  }
  glDrawElements( this->glDrawMode, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0 );
}

ScreenQuad::ScreenQuad(){
  Object::addSurfaceVN(
    Vertex( glm::vec3( -1.0, -1.0, 0.0 ), glm::vec2( 0.0, 0.0 ) ),
    Vertex( glm::vec3(  1.0, -1.0, 0.0 ), glm::vec2( 1.0, 0.0 ) ),
    Vertex( glm::vec3( -1.0,  1.0, 0.0 ), glm::vec2( 0.0, 1.0 ) )
  );

  Object::addSurfaceVN(
    Vertex( glm::vec3(  1.0,  1.0, 0.0 ), glm::vec2( 1.0, 1.0 ) ),
    Vertex( glm::vec3( -1.0,  1.0, 0.0 ), glm::vec2( 0.0, 1.0 ) ),
    Vertex( glm::vec3(  1.0, -1.0, 0.0 ), glm::vec2( 1.0, 0.0 ) )
  );

  Object::setDrawMode( GL_TRIANGLES );
}

/*void ScreenQuad::glSetTexture( int i, const std::string& filename ){
  Object::glSetTexture( i, filename );
}*/
