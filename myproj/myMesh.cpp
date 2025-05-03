#include "myMesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <GL/glew.h>
#include "myvector3d.h"

using namespace std;

myMesh::myMesh(void)
{
    /**** TODO ****/
}


myMesh::~myMesh(void)
{
    /**** TODO ****/
}

void myMesh::clear()
{
    for (unsigned int i = 0; i < vertices.size(); i++) if (vertices[i]) delete vertices[i];
    for (unsigned int i = 0; i < halfedges.size(); i++) if (halfedges[i]) delete halfedges[i];
    for (unsigned int i = 0; i < faces.size(); i++) if (faces[i]) delete faces[i];

    vector<myVertex*> empty_vertices;    vertices.swap(empty_vertices);
    vector<myHalfedge*> empty_halfedges; halfedges.swap(empty_halfedges);
    vector<myFace*> empty_faces;         faces.swap(empty_faces);
}

void myMesh::checkMesh()
{
    vector<myHalfedge*>::iterator it;
    for (it = halfedges.begin(); it != halfedges.end(); it++)
    {
        if ((*it)->twin == NULL)
            break;
    }
    if (it != halfedges.end())
        cout << "Error! Not all edges have their twins!\n";
    else cout << "Each edge has a twin!\n";
}


bool myMesh::readFile(std::string filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    name = filename;
    std::string s, t, u;
    std::vector<int> faceids;
    std::map<std::pair<int, int>, myHalfedge*> twin_map;

    while (std::getline(fin, s))
    {
        std::stringstream myline(s);
        myline >> t;

        if (t == "v") {
            float x, y, z;
            myline >> x >> y >> z;
            myPoint3D* p = new myPoint3D(x, y, z);
            myVertex* v = new myVertex();
            v->point = p;
            vertices.push_back(v);

            std::cout << "v " << x << " " << y << " " << z << std::endl;
        }
        else if (t == "f") {
            faceids.clear();
            while (myline >> u)
                faceids.push_back(std::stoi(u.substr(0, u.find("/"))) - 1);
            if (faceids.size() < 3) continue;

            myHalfedge** hedges = new myHalfedge * [faceids.size()];
            for (size_t i = 0; i < faceids.size(); ++i)
                hedges[i] = new myHalfedge();

            myFace* f = new myFace();
            f->adjacent_halfedge = hedges[0];

            for (size_t i = 0; i < faceids.size(); ++i)
            {
                int next = (i + 1) % faceids.size();
                int prev = (i + faceids.size() - 1) % faceids.size();

                hedges[i]->next = hedges[next];
                hedges[i]->prev = hedges[prev];

                auto key = std::make_pair(faceids[next], faceids[i]);
                auto it = twin_map.find(key);
                if (it != twin_map.end()) {
                    hedges[i]->twin = it->second;
                    it->second->twin = hedges[i];
                }
                else {
                    twin_map[{faceids[i], faceids[next]}] = hedges[i];
                }

                hedges[i]->source = vertices[faceids[i]];
                vertices[faceids[i]]->originof = hedges[i];

                halfedges.push_back(hedges[i]);
            }

            faces.push_back(f);
        }
    }

    checkMesh();
    normalize();

    return true;
}



void myMesh::computeNormals()
{
    /**** TODO ****/
}

void myMesh::normalize()
{
    if (vertices.size() < 1) return;

    int tmpxmin = 0, tmpymin = 0, tmpzmin = 0, tmpxmax = 0, tmpymax = 0, tmpzmax = 0;

    for (unsigned int i = 0; i < vertices.size(); i++) {
        if (vertices[i]->point->X < vertices[tmpxmin]->point->X) tmpxmin = i;
        if (vertices[i]->point->X > vertices[tmpxmax]->point->X) tmpxmax = i;

        if (vertices[i]->point->Y < vertices[tmpymin]->point->Y) tmpymin = i;
        if (vertices[i]->point->Y > vertices[tmpymax]->point->Y) tmpymax = i;

        if (vertices[i]->point->Z < vertices[tmpzmin]->point->Z) tmpzmin = i;
        if (vertices[i]->point->Z > vertices[tmpzmax]->point->Z) tmpzmax = i;
    }

    double xmin = vertices[tmpxmin]->point->X, xmax = vertices[tmpxmax]->point->X,
        ymin = vertices[tmpymin]->point->Y, ymax = vertices[tmpymax]->point->Y,
        zmin = vertices[tmpzmin]->point->Z, zmax = vertices[tmpzmax]->point->Z;

    double scale = (xmax - xmin) > (ymax - ymin) ? (xmax - xmin) : (ymax - ymin);
    scale = scale > (zmax - zmin) ? scale : (zmax - zmin);

    for (unsigned int i = 0; i < vertices.size(); i++) {
        vertices[i]->point->X -= (xmax + xmin) / 2;
        vertices[i]->point->Y -= (ymax + ymin) / 2;
        vertices[i]->point->Z -= (zmax + zmin) / 2;

        vertices[i]->point->X /= scale;
        vertices[i]->point->Y /= scale;
        vertices[i]->point->Z /= scale;
    }
}


void myMesh::splitFaceTRIS(myFace* f, myPoint3D* p)
{
    /**** TODO ****/
}

void myMesh::splitEdge(myHalfedge* e1, myPoint3D* p)
{

    /**** TODO ****/
}

void myMesh::splitFaceQUADS(myFace* f, myPoint3D* p)
{
    /**** TODO ****/
}


void myMesh::subdivisionCatmullClark()
{
    /**** TODO ****/
}


void myMesh::triangulate()
{
    /**** TODO ****/
}

//return false if already triangle, true othewise.
bool myMesh::triangulate(myFace* f)
{
    /**** TODO ****/
    return false;
}