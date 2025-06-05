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

	vector<myVertex *> empty_vertices;    vertices.swap(empty_vertices);
	vector<myHalfedge *> empty_halfedges; halfedges.swap(empty_halfedges);
	vector<myFace *> empty_faces;         faces.swap(empty_faces);
}

void myMesh::checkMesh()
{
	vector<myHalfedge *>::iterator it;
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
	string s, t, u;
	vector<int> faceids;
	myHalfedge** hedges;

	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Unable to open file!\n";
		return false;
	}
	name = filename;

	map<pair<int, int>, myHalfedge*> twin_map;
	map<pair<int, int>, myHalfedge*>::iterator it;

	while (getline(fin, s))
	{
		stringstream myline(s);
		myline >> t;
		if (t == "g") {}
		else if (t == "v")
		{
			float x, y, z;
			myline >> x >> y >> z;
			myPoint3D* p = new myPoint3D(x, y, z);
			myVertex* v = new myVertex();
			v->point = p;
			vertices.push_back(v);

			cout << "v " << x << " " << y << " " << z << endl;
		}
		else if (t == "mtllib") {}
		else if (t == "usemtl") {}
		else if (t == "s") {}
		else if (t == "f")
		{
			faceids.clear();
			cout << "f";
			while (myline >> u) {
				int id = atoi((u.substr(0, u.find("/"))).c_str()) - 1;
				faceids.push_back(id);
				cout << " " << id + 1;
			}
			cout << endl;

			if (faceids.size() < 3) continue;

			hedges = new myHalfedge * [faceids.size()];
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

				hedges[i]->adjacent_face = f;

				auto key = make_pair(faceids[next], faceids[i]);
				it = twin_map.find(key);
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


void myMesh::computeNormals() {

	/**** TODO ****/

	for (unsigned int i = 0; i < faces.size(); i++) {
		faces[i]->computeNormal();
	}

	
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i]->computeNormal();	
	}
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


void myMesh::splitFaceTRIS(myFace *f, myPoint3D *p)
{
	/**** TODO ****/
}

void myMesh::splitEdge(myHalfedge *e1, myPoint3D *p)
{

	/**** TODO ****/
}

void myMesh::splitFaceQUADS(myFace *f, myPoint3D *p)
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
		std::vector<myFace*> originalFaces = faces;
		faces.clear();

		for (myFace* polygon : originalFaces) {
			if (!polygon || !polygon->adjacent_halfedge) continue;

			std::vector<myHalfedge*> ring;
			myHalfedge* start = polygon->adjacent_halfedge;
			myHalfedge* current = start;
			do {
				ring.push_back(current);
				current = current->next;
			} while (current && current != start);

			int vertexCount = ring.size();
			if (vertexCount < 3) continue;
			if (vertexCount == 3) {
				faces.push_back(polygon);
				continue;
			}

			myVertex* pivot = ring[0]->source;

			for (int i = 1; i < vertexCount - 1; ++i) {
				myHalfedge* edge0 = (i == 1) ? ring[0] : new myHalfedge();
				myHalfedge* edge1 = ring[i];
				myHalfedge* edge2 = (i == vertexCount - 2) ? ring.back() : new myHalfedge();

				if (i != 1) {
					edge0->source = pivot;
					halfedges.push_back(edge0);
				}
				if (i != vertexCount - 2) {
					edge2->source = ring[i + 1]->source;
					halfedges.push_back(edge2);
				}

				edge0->next = edge1;
				edge1->next = edge2;
				edge2->next = edge0;

				edge0->prev = edge2;
				edge1->prev = edge0;
				edge2->prev = edge1;

				myFace* triangle = (i == 1) ? polygon : new myFace();
				triangle->adjacent_halfedge = edge0;
				if (!triangle->normal) triangle->normal = new myVector3D(0.0, 0.0, 0.0);

				edge0->adjacent_face = triangle;
				edge1->adjacent_face = triangle;
				edge2->adjacent_face = triangle;

				faces.push_back(triangle);
			}
		}
		computeNormals();

}

//return false if already triangle, true othewise.
bool myMesh::triangulate(myFace *f)
{
	/**** TODO ****/
	return false;

}




