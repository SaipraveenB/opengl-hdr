
#include "stdgl.h"
#include "scene.h"
#include "camera.h"
#include "loader.h"
#include "render_target.h"

#include "skybox.h"
#include "fx_phong.h"
#include "fx_directdraw.h"
#include "fx_hdr.h"
#include "fx_lpass.h"
#include "fx_bloom.h"
#include "fx_combine.h"
#include "fx_project.h"
#include "fx_shadow.h"
#include "fx_scatter.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

GLuint vbo;
GLuint vbo2;

GLuint vao;
GLuint vao2;

GLuint shader;
GLFWwindow* window;

GLuint idbuf;

ModelCamera* cam;

Scene* pScene;
Scene* pLightScatterCone;

static const GLenum GL_CUBEMAP_TYPES[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

  // Shader uniform variable location storage.
  GLint uCol;
  GLint mWorld;
  GLint mView;
  GLint mProj;
  GLint mWVP;
  GLint tColor;
  GLint vLight;
  GLint uAmbient;
  GLint vTexU;
  GLint vTexV;

  FXPhong* pTech;
  FXDirectDrawMS* pDDTech;
  FXLightPass* pLPass;
  Skybox* pSkyTech;
  FXHDRMS* pHDRTech;

  FXBloomH* pBloomTechH;
  FXBloomV* pBloomTechV;

  FXCombine* pCombineTech;
  FXProject* pProjectTech;
  FXShadow* pShadowTech;
  FXScatter* pScatterTech;

  PointLight* pLight0;
  glm::mat4* pLight0SHM;
  PointLight* pLight1;
  AmbientLight* pAmbient;
  SpotLight* pSpotLight;
  glm::mat4 pSpotLightSHM;

  ScreenQuad* pSQuad;
  RenderTarget* pTarget;
  RenderTarget* pDownsampled;
  RenderTarget* pHighlight;
  RenderTarget* pBloomH;
  RenderTarget* pBloomHV;
  RenderTarget* pLDR;

  // Soft shadows for the point light.
  RenderTarget* pShadows;
  RenderTarget* pShadowsH;
  RenderTarget* pShadowsHV;
  CubemapRenderTarget* pShadowTarget;

  RenderTarget* pSpotShadowTarget;



  GradientCube* pCube;


  int tmWidth;
  int tmHeight;
  int tmLvl;

  // Global Constants predeclared to save some compute power.
  float color1[] = {0.5,0.5,0.1,0.5};
  float color2[] = {0.0,1.0,0.5,1.0};
  glm::vec3 lightPos = glm::vec3( 0.0f, 0.0f, 5.0f );
  glm::vec3 texU = glm::vec3( 0.0f, 1.0f, -0.0f );
  glm::vec3 texV = glm::vec3( 1.0f, 0.0f, 0.0f );
  float fAmbient = 0.04;
  // Frame counter
  int numFrames = 0;
  float fLightAngularSpeed = 60.0f;

  glm::mat4 matWorld, matProj;


  float fExposure;

  Scene* makeScene(){
    ModelLoader loader;
    Object* o = loader.load("teapot.obj", true);
    o->setDrawMode( GL_TRIANGLES );// Render mode: triangluar mesh
    o->glSetTexture(0, "test0.png");// Texture
    o->glSetTexture(1, pSkyTech->getTexture() );  // Environment map.
    o->glSetTexture(2, pShadowTarget->getColorTexture() );  // Blurred shadows-only image. for soft-shadows.
    o->glSetTexture(3, pShadowsHV->getColorTexture() );
    o->glSetTexture(0, new Texture( GL_TEXTURE_2D, pCube->getTexture() ) );
    Scene* sc = new Scene();
    sc->addObject(o);
    return sc;
  }

  float iFrameTime = 0.0f;


  void toneMapCoordinates( int width, int height, int* levels, int* wOut, int* hOut ){
    int lvl = (int)floor( log2( (double)min(width,height) ) );

    *wOut = (int)floor( ((double)width) / pow( 2, lvl ) );
    *hOut = (int)floor( ((double)height) / pow( 2, lvl ) );
    *levels = lvl;
  }


  glm::mat4* getShadowMapWVP( PointLight* l ){
    glm::vec3 pos = l->getPosition();
    glm::mat4* m = new glm::mat4[6];
    m[0] = ( glm::lookAt( pos, pos + glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, -1.0, 0.0 ) ) );
    m[1] = ( glm::lookAt( pos, pos + glm::vec3( -1.0, 0.0, 0.0 ), glm::vec3( 0.0, -1.0, 0.0 ) ) );
    m[2] = ( glm::lookAt( pos, pos + glm::vec3( 0.0, 1.0, 0.0 ), glm::vec3( 0.0, -1.0, 0.0 ) ) );
    m[3] = ( glm::lookAt( pos, pos + glm::vec3( 0.0, -1.0, 0.0 ), glm::vec3( 0.0, -1.0, 0.0 ) ) );
    m[4] = ( glm::lookAt( pos, pos + glm::vec3( 0.0, 0.0, 1.0 ), glm::vec3( 0.0, -1.0, 0.0 ) ) );
    m[5] = ( glm::lookAt( pos, pos + glm::vec3( 0.0, 0.0, -1.0 ), glm::vec3( 0.0, -1.0, 0.0 ) ) );

    return m;
  }


  void mainLoop(){

    // Get frame descriptor
    float deltaTime =  - ( ( iFrameTime ) - ( iFrameTime = glfwGetTime() ) );

    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Enable color bits output.
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    //glStencilMask( 0xFF );


    // Clear it to 0 initially.
    //glClear( GL_STENCIL_BUFFER_BIT );


    cam->glfwHandleCursor();
    glm::mat4 matView = cam->getViewMatrix();

    pTech->setView( matView );
    pSkyTech->setView( matView );
    pShadowTech->setView( matView );
    pScatterTech->setView( matView );

    pLight0->setPosition( lightPos );

    pLight1->setPosition( -lightPos );

    pLight0->setColor( glm::vec4(1.0,1.0,1.0,0.70) );
    pLight1->setColor( glm::vec4(1.0,1.0,1.0,0.20) );


    pLight0SHM = getShadowMapWVP( pLight0 );
    pSpotLightSHM = pSpotLight->getSHMViewMatrix();

    pAmbient->setColor( glm::vec4( fAmbient, fAmbient, fAmbient, 1.0 ) );


    pProjectTech->glActivate();

    pShadowTarget->glBind();

    for( int i = 0; i < 6; i++ ){
      pProjectTech->setView( pLight0SHM[i] );
      pProjectTech->glTransferUniforms();
      pShadowTarget->glChangeFace( GL_CUBEMAP_TYPES[i] );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      pScene->glDraw();
    }

    pProjectTech->setView( pSpotLightSHM );
    pProjectTech->glTransferUniforms();
    pSpotShadowTarget->glBind();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    pScene->glDraw();


    /*glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glViewport( 0, 0, 640, 480 );

    pScatterTech->glActivate();
    pScatterTech->setWVPSHM( glm::transpose( glm::perspective( 90.0f, 1.0f, 1.0f, 1000.0f ) ) * pSpotLightSHM );
    pScatterTech->glTransferUniforms();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    pLightScatterCone->glDraw();*/


    pShadowTech->glActivate();
    pShadowTech->setLightPos( pLight0->getPosition() );
    pShadowTech->glTransferUniforms();
    pShadows->glBind();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    pScene->glDraw();


    pBloomTechH->glPass( pShadows->getColorTexture(), pShadowsH, pSQuad );

    pBloomTechV->glPass( pShadowsH->getColorTexture(), pShadowsHV, pSQuad );


    //pBloomTechV->glActivate();
    //pBloomTechV->glTransferUniforms();
    //pSQuad->glSetTexture( 0, pShadowsH->getColorTexture() );
    //pSQuad->glSetTexture( 1, pBloomHV->getColorTexture() );
    //pSQuad->glDraw();
    //pSQuad->glSetTexture( 1, NULL );


    // Bind the secondary texture.
    pTarget->glBind();
    //glViewport( 0,0, 640, 480 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Geometric Effect: a technique which requires internal rendering. thus requires a draw call.
    pSkyTech->glDraw();

    // Note the depth buffer still persists. SkyTech sets it to an even 0.5.
    // So, we clear depth before proceeding.
    glClear( GL_DEPTH_BUFFER_BIT );

    pTech->glActivate();
    pTech->glTransferUniforms();
    pScene->glDraw();


    pDDTech->glPass( pTarget->getColorTexture(), pDownsampled, pSQuad );


    // The following lines calculate average lighting in the scene for tone mapping.
    pDownsampled->getColorTexture()->glBind();
    float data[100];
    glGetTexImage( GL_TEXTURE_2D, tmLvl, GL_RGBA, GL_FLOAT, data );
    float avg = 0.0f;
    for( int i = 0; i < tmWidth * tmHeight * 4; i++ ){
      avg += data[i] / tmWidth * tmHeight * 4;
    }
    //std::cout<<"Luminance:"<<avg<<std::endl;
    float fActualExposure = 144.0f / (avg * avg);
    float fApproachFactor = 0.05f;
    float fExposure = fExposure + (fActualExposure - fExposure)*fApproachFactor;
    pHDRTech->setExposure( fExposure );

    pHDRTech->glPass( pTarget->getColorTexture(), pLDR, pSQuad );

    pLPass->glPass( pDownsampled->getColorTexture(), pHighlight, pSQuad );

    pBloomTechH->glPass( pHighlight->getColorTexture(), pBloomH, pSQuad );

    pBloomTechV->glPass( pBloomH->getColorTexture(), pBloomHV, pSQuad );




    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glViewport( 0, 0, 1280, 960 );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    pCombineTech->glActivate();
    pCombineTech->glTransferUniforms();
    pSQuad->glSetTexture( 0, pLDR->getColorTexture() );
    pSQuad->glSetTexture( 1, pBloomHV->getColorTexture() );
    pSQuad->glDraw();
    pSQuad->glSetTexture( 1, NULL );

    glfwPollEvents();

    glfwSwapBuffers( window );

    numFrames++;
    if( !(numFrames%50) )
      std::cout<<numFrames<<std::endl;

    }


    /*
    *  C++ Main function.
    * */

    // MAIN
    int main (int argc, char** argv) {
      Magick::InitializeMagick(argv[0]);
      // start GL context and O/S window using the GLFW helper library
      if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
      }
      // Demand OpenGL 4.1
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  #ifdef __APPLE__
      // Use Core profile to obtain a context for the latest OpenGL spec.
      // Otherwise we're stuck at 2.1
      std::cout<<"Apple FTW\n";
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif


      window = glfwCreateWindow (1280, 960, "Hello Triangle", NULL, NULL);
      if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
      }
      glfwSetWindowSize( window, 1280/2 , 960/2);

      glfwWindowHint(GLFW_SAMPLES, 4);
      glEnable( GL_MULTISAMPLE );

      glfwMakeContextCurrent (window);
      // start GLEW extension handler
      glewExperimental = GL_TRUE;
      glewInit();

      // get version info
      const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
      const GLubyte* version = glGetString (GL_VERSION); // version as a string
      printf ("Renderer: %s\n", renderer);
      printf ("OpenGL version supported %s\n", version);



      glEnable( GL_DEPTH_TEST );
      glDepthFunc( GL_LEQUAL );

      pTech = new FXPhong();
      pDDTech = new FXDirectDrawMS( 4 );  // Configure direct draw shaders to average 16 samples.
      pHDRTech = new FXHDRMS( 4 );  //  HDR rendering technique to convert a given HDR texture to an LDR output. Multisampled to 16 samples per pixel.
      pBloomTechH = new FXBloomH( 20 ); // Render bright area horizontal bloom.
      pBloomTechV = new FXBloomV( 20 ); // Render bright area vertical bloom.
      pCombineTech = new FXCombine();
      pProjectTech = new FXProject();
      pShadowTech = new FXShadow();
      pScatterTech = new FXScatter();
      pLPass = new FXLightPass( 5.0f );
      pSkyTech = new Skybox();
      pLight0 = new PointLight();
      pLight1 = new PointLight();
      pSpotLight = new SpotLight();
      pAmbient = new AmbientLight();
      pSQuad = new ScreenQuad();
      pCube = new GradientCube( 50, 50, 50 );
      pTarget = new RenderTarget( 1280, 960, GL_RGBA32F, GL_RGBA, GL_FLOAT, 4, false );// Configure target to accumulate 4 samples.
      //pTarget = new RenderTarget( 640, 480, GL_RGBA8, GL_RGBA, GL_UNSIGNED_SHORT, 4 );// Configure target to accumulate 4 samples.
      pDownsampled = new RenderTarget( 320, 240, GL_RGBA32F, GL_RGBA, GL_FLOAT, 0, true );
      pHighlight = new RenderTarget( 320, 240, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pBloomH = new RenderTarget( 1280, 960, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pBloomHV = new RenderTarget( 1280, 960, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pLDR = new RenderTarget( 1280, 960, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pShadowTarget = new CubemapRenderTarget( 512, 512, GL_RGBA32F, GL_RGBA, GL_FLOAT );
      pShadows = new RenderTarget( 1280, 960, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pShadowsH = new RenderTarget( 1280, 960, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pShadowsHV = new RenderTarget( 1280, 960, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );
      pSpotShadowTarget = new RenderTarget( 512, 512, GL_RGBA32F, GL_RGBA, GL_UNSIGNED_BYTE, 0, false );


      pScatterTech->setLight( pSpotLight );

      pSQuad->glTransfer();
      pSQuad->glSetTexture( 0, pTarget->getColorTexture() );

      matProj = glm::transpose( glm::perspective( 45.0f, 640.0f/480.0f, 1.0f, 1000.0f ) );
      matWorld = glm::mat4();

      pTech->setWorld( matWorld );
      pTech->setProj( matProj );
      pTech->setView( glm::mat4() );

      std::cout<<"Setting project tech parameters";fflush(stdout);
      pProjectTech->setWorld( glm::mat4() );
      pProjectTech->setProj( glm::transpose( glm::perspective( 90.0f, 1.0f, 1.0f, 1000.0f ) ) );
      pProjectTech->setView( glm::mat4() );

      pScatterTech->setWorld( matWorld );
      pScatterTech->setProj( matProj );
      pScatterTech->setView( glm::mat4() );

      std::cout<<"Setting Exposure";fflush(stdout);

      fExposure = 1.0f;
      pHDRTech->setExposure( 1.0f );  // Initialize Exposure.

      pSkyTech->setWorld( matWorld );
      pSkyTech->setProj( matProj );
      pSkyTech->setView( glm::mat4() );

      pShadowTech->setWorld( matWorld );
      pShadowTech->setProj( matProj );
      pShadowTech->setView( glm::mat4() );


      pTech->addLightP( pSpotLight );
      //pTech->addLightP( pLight1 );
      pTech->setLightA( pAmbient );
      pTech->setSpecularExponent( 32.0f );
      pTech->setIntensityPackingConstant( 300.0f );
      pSkyTech->setTexture("skybox/ame_mudskip","front.tga","back.tga","top.tga","down.tga","right.tga","left.tga");
      //pSkyTech->setTexture( pShadowTarget->getColorTexture() );
      pSkyTech->glTransfer();

      pScene = makeScene();
      pScene->glTransfer();

      cam = new ModelCamera( window );
      cam->setSpeedX( -0.03f );
      cam->setSpeedY( 0.03f );
      cam->setRadius( 5.0f );
      cam->setOrientation( glm::vec3( 0.0f, -1.0f, 0.0f ) );
      cam->reset( glm::vec3( 0.0f, 0.0f, 1.0f ) );

      std::cout<<"Camera Ready"<<std::endl;

      toneMapCoordinates( 320, 240, &tmLvl, &tmWidth, &tmHeight );

      std::cout<<"Tone mapping references: @level:"<<tmLvl<<" @width:"<<tmWidth<<" @height:"<<tmHeight<<std::endl;

      pLight0SHM = getShadowMapWVP( pLight0 );


      pSpotLight->setPosition( lightPos );
      pSpotLight->setColor( glm::vec4(1.0f,1.0f,1.0f,0.80f) );
      pSpotLight->setConstants( glm::vec4(0.1f,0.0f,0.0f,0.0f) );
      pSpotLight->setHalfAngle( 3.1415f / 8.0f );
      pSpotLight->setDirection( glm::vec3( 0.0f, 0.0f, -1.0f ) );

      pLightScatterCone = new Scene();
      Object* slCone = pSpotLight->getBoundingGeometry( 19 );
      slCone->glSetTexture( 0, pSpotShadowTarget->getColorTexture() );
      pLightScatterCone->addObject( slCone );
      pLightScatterCone->glTransfer();


      glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
      //glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
      //glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

      while (!glfwWindowShouldClose(window)) {
        mainLoop();
      }

      glfwTerminate();
      return 0;
    }
