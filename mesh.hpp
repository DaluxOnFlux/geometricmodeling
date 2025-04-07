#include <vector>
#include <string>

class myVertex;
class myHalfedge;
class myFace;

class myVertex {
public:
    double x, y, z;
    myHalfedge* edge;
};

class myHalfedge {
public:
    myVertex* origin;
    myHalfedge* twin;
    myHalfedge* next;
    myFace* face;
};

class myFace {
public:
    myHalfedge* edge;
};

class myMesh {
public:
    std::vector<myVertex*> vertices;
    std::vector<myHalfedge*> halfedges;
    std::vector<myFace*> faces;

    void readFile(const std::string& filename);
    void computeNormals();
    void triangulate();
};
