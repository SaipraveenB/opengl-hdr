#include "loader.h"
#include <iostream>
#include "object.h"

ModelLoader::ModelLoader(){

}
Object* ModelLoader::load( const std::string& filename, bool bVtxOnly ){

  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > vVertices;
  std::vector< glm::vec2 > vUVs;
  std::vector< glm::vec3 > vNormals;

  Object* pObject = new Object();

  const std::string str = "r";
  FILE * file = fopen(filename.c_str(), "r");
  if( file == NULL ){
    return NULL;
  }

  while( 1 ){

    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF){
      break; // EOF = End Of File. Quit the loop.
    }

      // else : parse lineHeader
    if ( strcmp( lineHeader, "v" ) == 0 ){

      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
      vVertices.push_back(vertex);
      std::cout<<"READ VTX: "<<vertex.x<<","<<vertex.y<<","<<vertex.z<<std::endl;
    }else if ( strcmp( lineHeader, "vt" ) == 0 ){

      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y );
      vUVs.push_back(uv);
      std::cout<<"READ UV: "<<uv.x<<","<<uv.y<<","<<std::endl;
    }else if ( strcmp( lineHeader, "vn" ) == 0 ){

      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
      vNormals.push_back(normal);
      std::cout<<"READ NORMAL: "<<normal.x<<","<<normal.y<<","<<normal.z<<std::endl;
    }else if ( strcmp( lineHeader, "f" ) == 0 ){

      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = 0;
      if( bVtxOnly )
        fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
      else
        fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

      /*if (matches != 9){
        return NULL;
      }*/

      // Auto compute normals.

      if( bVtxOnly )
        pObject->addSurfaceVN(
          Vertex( vVertices[vertexIndex[0]-1], glm::vec2(0.0f,0.0f) ),
          Vertex( vVertices[vertexIndex[1]-1], glm::vec2(0.0f,0.0f) ),
          Vertex( vVertices[vertexIndex[2]-1], glm::vec2(0.0f,0.0f) )
        );
      else
        pObject->addSurfaceV(
          Vertex( vVertices[vertexIndex[0]-1], vUVs[uvIndex[0]-1], vNormals[normalIndex[0]-1] ),
          Vertex( vVertices[vertexIndex[1]-1], vUVs[uvIndex[1]-1], vNormals[normalIndex[1]-1] ),
          Vertex( vVertices[vertexIndex[2]-1], vUVs[uvIndex[2]-1], vNormals[normalIndex[2]-1] )
        );

      std::cout<<"READ SURF: "<<vertexIndex[0]<<","<<vertexIndex[1]<<","<<vertexIndex[2]<<std::endl;
    }
  }

  std::cout<<"Load finished. "<<pObject->getIndexCount()/3<<" surfaces read.";
  return pObject;
}
