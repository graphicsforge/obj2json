
#include "Vertex.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

Vertex::Vertex(vector_t *_vertex, vector_t *_texcoord, vector_t *_normal)
{
  vertex = _vertex;
  texcoord = _texcoord;
  normal = _normal;
}

Vertex::~Vertex()
{
}

Vertex::vector_t Vertex::parseObjLine(char* line)
{
  char *word = strtok( line, " " );
  uint8_t index = 0;
  // make default vector_t
  Vertex::vector_t ret;
  ret.x = 0;
  ret.y = 0;
  ret.z = 0;
  ret.w = 1;
  float *iterator = (float*)&ret;
  while ( word ) {
    *iterator = atof(word);
    if ( ++index>=3 ) // currently not supporting additional numbers
      break;
    // on to the next one
    iterator+=1;
    word = strtok( NULL, " " );
  }
  return ret;
}

bool Vertex::operator==(const Vertex &rhs)
{
  return this->vertex==rhs.vertex &&
         this->texcoord==rhs.texcoord &&
         this->normal==rhs.normal;
}
