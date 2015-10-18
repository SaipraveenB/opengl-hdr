#include "technique.h"
#include <fstream>
/*class AbstractTechnique{

private:
  std::string vsFile;
  std::string fsFile;

  virtual void glLinkUniforms();

public:
*/
void AbstractTechnique::loadShader( GLuint obj, const char* filename ){


  std::ifstream file(filename);
  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  char* prog = new char[size+1];
  const char* cprog = prog;

  //Manually add null character to prevent junk data.
  prog[size] = '\0';
  if (file.read( prog, size ))
  {
    std::cout<<"Shader slurped from file \'"<<filename<<"\': "<<std::endl;
    std::cout<<prog<<std::endl;
    // Load shader source as a string to the Shader object.
    glShaderSource (obj, 1, &(cprog), NULL);
    // Compile the shader object.
    glCompileShader (obj);

  }
  file.close();
}
void AbstractTechnique::glActivate(){
  glUseProgram( shader );
}

void AbstractTechnique::glTransferUniforms(){

}

AbstractTechnique::AbstractTechnique(){

}
AbstractTechnique::~AbstractTechnique(){

}

void AbstractTechnique::glPass( Texture* pSource, RenderTarget* pDest, ScreenQuad* pSQuad ){
  if( pDest )
    pDest->glBind();
  else{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glViewport( 0,0, 1280, 720 );// BAD, BAD IDEA... change ASAP.
  }

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  this->glActivate();
  this->glTransferUniforms();
  pSQuad->glSetTexture( 0, pSource );
  pSQuad->glDraw();
  if( pDest )
    pDest->generateMipmaps();
}
void AbstractTechnique::load( const std::string& str_vs, const std::string& str_fs ){
    // Create a vertex shader object currently empty.
  GLuint vs = glCreateShader (GL_VERTEX_SHADER);
  loadShader( vs, str_vs.c_str() );
    // Similarly we create the fragment shader object.
  GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
  loadShader( fs, str_fs.c_str() );

    // Now create a shader program that links a pixel and a vertex shader.
  shader = glCreateProgram();
  glAttachShader( shader, fs );
  glAttachShader( shader, vs );

  // Link program links together the variables in FS and VS. Also creates dockets for uniform parameters so we can set them.
  glLinkProgram( shader );
}


  void WVPBasedTechnique::setWorld( glm::mat4 m ){
    matWorld = m;
  }
  void WVPBasedTechnique::setView( glm::mat4 m ){
    matView = m;
  }
  void WVPBasedTechnique::setProj( glm::mat4 m ){
    matProj = m;
  }

  /*WVPBasedTechnique::WVPBasedTechnique(){

  }

  WVPBasedTechnique::~WVPBasedTechnique(){

  }*/

  void WVPBasedTechnique::glLinkUniforms(){
    mWorld = glGetUniformLocation( shader, "mWorld" );
    mView = glGetUniformLocation( shader, "mView" );
    mProj = glGetUniformLocation( shader, "mProj" );
    mWVP = glGetUniformLocation( shader, "mWVP" );
  }
  void WVPBasedTechnique::glTransferUniforms(){
    glm::mat4 matWVP = getWVP();

    if( mWVP != -1 ){
      glUniformMatrix4fv( mWVP, 1, GL_FALSE, glm::value_ptr(matWVP) );
    }
    if( mView != -1 ){
      glUniformMatrix4fv( mView, 1, GL_FALSE, glm::value_ptr(matView) );
    }
    if( mWorld != -1 ){
      glUniformMatrix4fv( mWorld, 1, GL_FALSE, glm::value_ptr(matWorld) );
    }
    if( mProj != -1 ){
      glUniformMatrix4fv( mProj, 1, GL_FALSE, glm::value_ptr(matProj) );
    }
  }
  glm::mat4 WVPBasedTechnique::getWorld(){ return matWorld; }
  glm::mat4 WVPBasedTechnique::getView(){ return matView; }
  glm::mat4 WVPBasedTechnique::getProj(){ return matProj; }
  glm::mat4 WVPBasedTechnique::getWVP(){ return matProj * matView * matWorld; }
