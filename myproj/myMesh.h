#pragma once  
#include "myFace.h"  
#include "myHalfedge.h"  
#include "myVertex.h"  
#include "myPoint3D.h"
#include <vector>  
#include <string>  
#include <map>  
#include <algorithm>  

class myMesh
{
public:
    std::vector<myVertex*> vertices;
    std::vector<myHalfedge*> halfedges;
    std::vector<myFace*> faces;
    std::string name;

    myMesh();
    ~myMesh();

    void checkMesh();
    bool checkFace();
    bool readFile(std::string filename);
    void computeNormals();
    void normalize();
    void simplify();
    void clear();

    void subdivisionCatmullClark();

    void computeFacePoints(std::map<myFace*, myVertex*>& milieuxFaces);
    void computeEdgeMidpoints(std::map<myHalfedge*, myVertex*>& milieuxAretes, std::map<myFace*, myVertex*>& milieuxFaces);
    void updateVertices(const std::map<myFace*, myVertex*>& milieuxFaces);
    void createNewElements(myFace* oldFace, std::vector<myFace*>& nvFaces, std::vector<myHalfedge*>& nvHalfedges, std::vector<myVertex*>& nvVertices, std::map<myFace*, myVertex*>& facePoints, std::map<myHalfedge*, myVertex*>& edgePoints);
    void assignTwinsForNewEdges(const std::vector<myHalfedge*>& nouvellesAretes);
    void addVertexIfNotExists(std::vector<myVertex*>& nouveauxSommets, myVertex* vertex);
    myVertex* computeFacePoint(myFace* f);

    void triangulate();
    bool triangulate(myFace*);
};
