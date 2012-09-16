
class Vertex{

  public:
    struct {
      float x, y, z, w;
    } typedef vector_t;

    vector_t *vertex, *texcoord, *normal;

    // TODO
    int parent;  float weight;

    // this class just keeps a bunch of vector_t pointers
    Vertex(vector_t *vertex, vector_t *texcoord, vector_t *normal);
    ~Vertex();
    // a helper function to make vector_ts
    static vector_t parseObjLine(char*);
    // see if two Vertexes are the same
    bool operator==(const Vertex &rhs); 
};


