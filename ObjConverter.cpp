
// standard includes
#include <cstdlib>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
// class includes
#include "Vertex.hpp"

/*!
 * prints commandline options
 */
void ShowUsage()
{
  fprintf( stdout, "Usage is:\n"
         "objConverter -i inputfile.obj -o outputfile.json\n"
        );
  return;
}

int main(int argc, char *argv[])
{
  char* inputFilename = NULL;
  char* outputFilename = NULL;

  static struct option long_options[] = 
    {
      {"import", required_argument, 0, 0},
      {"export", required_argument, 0, 0},
      {"help", no_argument, 0, 0},
      {0, 0, 0, 0}
    };
  char opt;
  int option_index = 0;
  opterr = 0;  
  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, &option_index)) != -1)
  {
    switch(opt)
    {
      case 0:
        // option set flag
        if ( !strcmp(long_options[option_index].name, "import") )
          inputFilename = optarg;
        else if ( !strcmp(long_options[option_index].name, "export") )
          outputFilename = optarg;
        else if ( !strcmp(long_options[option_index].name, "help") )
          ShowUsage();
        break;
      case 'i':
        inputFilename = optarg;
        break;
      case 'o':
        outputFilename = optarg;
        break;
      case '?':
        fprintf( stderr, "unknown option %s\n", argv[optind] );
      case 'h':
      default:
        ShowUsage();
        exit(0);
        break;
    }
  }

  // grab our input file
  FILE *inFile = fopen( inputFilename, (char*)"r" );
  if ( !inFile )
  {
    if ( inputFilename )
    {
      fprintf( stderr, "ERROR: could not open %s\n", inputFilename );
      exit(1);
    }
    else
      inFile = fopen( argv[1], (char*)"r" );
    if ( !inFile )
    {
      fprintf( stderr, "ERROR: you must specify an input (--import)\n" );
      exit(1);
    }
  }
  // grab our output file
  FILE *outFile = fopen( outputFilename, (char*)"w" );
  if ( !outFile )
    outFile = stdout;

  char buffer[1023];
  char *type, *values, *word;
  std::vector<Vertex::vector_t> positions;
  std::vector<Vertex::vector_t> texcoords;
  std::vector<Vertex::vector_t> normals;
  std::vector<Vertex> vbo;
  std::vector<uint64_t> ibo;
  while ( fgets ( buffer, sizeof(buffer), inFile ) != NULL )
  {
    // parse out the first word to determine the type
    type = strtok( buffer, " " );
    values = buffer+strlen(buffer)+1;
    // parse out our values
    if ( type[0]=='v' )
    {
      Vertex::vector_t vector = Vertex::parseObjLine(values);
      if ( !strcmp(type, (char*)"v" ) )
        positions.push_back( vector );
      else if ( !strcmp(type, (char*)"vt" ) )
        texcoords.push_back( vector );
      else if ( !strcmp(type, (char*)"vn" ) )
        normals.push_back( vector );
    }
    else if ( !strcmp(type, (char*)"f" ) )
    {
      word = strtok( values, " " );
      // TODO handle quads
      while ( word )
      {
        int64_t vIndex=-1, tIndex=-1, nIndex=-1;
        // TODO detect if normals aren't specified and building them here
        sscanf( word, "%li/%li/%li", &vIndex, &tIndex, &nIndex );
        Vertex vertex(&positions[vIndex-1], &texcoords[tIndex-1], &normals[nIndex-1]);
        // see if we already have this vertex
        int64_t vboIndex = -1;
        for ( uint32_t i=0; i<vbo.size(); i++ )
          if ( vertex==vbo[i] ) {
            vboIndex = i;
            ibo.push_back(i);
            break;
          }
        if ( vboIndex<0 ) {
          vbo.push_back(vertex);
          ibo.push_back((vbo.end()-1)-vbo.begin());
        }
        word = strtok( NULL, " " );
      }
    }
  }

  // output JSON
  fprintf( outFile, "{\n" );
  // print out vertexes
  fprintf( outFile, "\"vertexes\":[\n" );
  for ( uint32_t i=0; i<vbo.size(); i++ )
  {
    if ( i>0 )
      fprintf( outFile, ",\n" );
    fprintf( outFile, "%f, %f, %f, %f, %f, %f, %f, %f", vbo[i].vertex->x, vbo[i].vertex->y, vbo[i].vertex->z, vbo[i].normal->x, vbo[i].normal->y, vbo[i].normal->z, vbo[i].texcoord->x, vbo[i].texcoord->y );
  }
  fprintf( outFile, "],\n" );
  fprintf( outFile, "\"indexes\":[\n" );
  for ( uint32_t i=0; i<ibo.size(); i++ )
  {
    if ( i>0 )
      fprintf( outFile, ", " );
    fprintf( outFile, "%lu", ibo[i] );
  }
  fprintf( outFile, "]\n" );
  // we're done
  fprintf( outFile, "}" );

}

