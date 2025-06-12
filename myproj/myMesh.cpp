#include "myMesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <GL/glew.h>
#include "myvector3d.h"
#include "myMesh.h"

using namespace std;

myMesh::myMesh(void)
{
	/**** TODO ****/
}


myMesh::~myMesh(void)
{
	/**** TODO ****/
}

void myMesh::computeFacePoints(std::map<myFace*, myVertex*>& milieuxFaces) {
	for (myFace* faceCourante : faces) {
		myVertex* milieu = computeFacePoint(faceCourante);
		milieuxFaces[faceCourante] = milieu;
	}
}

myVertex* myMesh::computeFacePoint(myFace* f) {
	myVertex* v = new myVertex();
	v->point = new myPoint3D();
	myHalfedge* edge = f->adjacent_halfedge;
	myHalfedge* courant = edge;
	int count = 0;

	do {
		*(v->point) += *(courant->source->point);
		courant = courant->next;
		count++;
	} while (courant != edge);

	*(v->point) /= count;
	return v;
}

void myMesh::computeEdgeMidpoints(std::map<myHalfedge*, myVertex*>& milieuxAretes, std::map<myFace*, myVertex*>& milieuxFaces) {
	for (myHalfedge* he : halfedges) {
		if (milieuxAretes.find(he) == milieuxAretes.end() && milieuxAretes.find(he->twin) == milieuxAretes.end()) {
			myVertex* v = new myVertex();
			v->point = new myPoint3D();

			myPoint3D pt1 = *(he->source->point);
			myPoint3D pt2 = *(he->twin->source->point);

			if (he->adjacent_face && he->twin->adjacent_face) {
				myPoint3D pt3 = *(milieuxFaces[he->adjacent_face]->point);
				myPoint3D pt4 = *(milieuxFaces[he->twin->adjacent_face]->point);
				*(v->point) = (pt1 + pt2 + pt3 + pt4) / 4;
			}

			milieuxAretes[he] = v;
			milieuxAretes[he->twin] = v;
		}
	}
}

void myMesh::updateVertices(const std::map<myFace*, myVertex*>& milieuxFaces) {
	for (myVertex* sommet : vertices) {
		myPoint3D* origine = new myPoint3D(*(sommet->point));
		myPoint3D* moyFaces = new myPoint3D();
		myPoint3D* moyAretes = new myPoint3D();

		int n = 0;
		myHalfedge* start = sommet->originof;
		myHalfedge* courant = start;

		do {
			*moyFaces += *milieuxFaces.at(courant->adjacent_face)->point;
			*moyAretes += (*(courant->source->point) + *(courant->twin->source->point)) / 2;
			courant = courant->twin->next;
			n++;
		} while (courant != start);

		*moyFaces /= n;
		*moyAretes /= n;
		*(sommet->point) = (*moyFaces + (*moyAretes * 2) + (*origine * (n - 3))) / n;

		delete origine;
	}
}

void myMesh::createNewElements(myFace* oldFace, std::vector<myFace*>& nvFaces, std::vector<myHalfedge*>& nvHalfedges, std::vector<myVertex*>& nvVertices, std::map<myFace*, myVertex*>& facePoints, std::map<myHalfedge*, myVertex*>& edgePoints) {
	myHalfedge* start = oldFace->adjacent_halfedge;
	myHalfedge* courant = start;

	do {
		myFace* newFace = new myFace();
		std::vector<myHalfedge*> he(4);
		for (int i = 0; i < 4; ++i) he[i] = new myHalfedge();

		myVertex* v0 = courant->source;
		myVertex* v1 = edgePoints[courant];
		myVertex* v2 = facePoints[oldFace];
		myVertex* v3 = edgePoints[courant->prev];

		he[0]->source = v0;
		he[1]->source = v1;
		he[2]->source = v2;
		he[3]->source = v3;

		for (int i = 0; i < 4; ++i) {
			he[i]->adjacent_face = newFace;
			he[i]->source->originof = he[i];
			he[i]->next = he[(i + 1) % 4];
			he[i]->prev = he[(i + 3) % 4];
		}

		newFace->adjacent_halfedge = he[0];
		nvFaces.push_back(newFace);
		nvHalfedges.insert(nvHalfedges.end(), he.begin(), he.end());
		addVertexIfNotExists(nvVertices, v0);
		addVertexIfNotExists(nvVertices, v1);
		addVertexIfNotExists(nvVertices, v2);
		addVertexIfNotExists(nvVertices, v3);

		courant = courant->next;
	} while (courant != start);
}

void myMesh::assignTwinsForNewEdges(const std::vector<myHalfedge*>& nouvellesAretes) {
	for (myHalfedge* he : nouvellesAretes) {
		for (myHalfedge* candidate : nouvellesAretes) {
			if (he->source == candidate->next->source && he->next->source == candidate->source) {
				he->twin = candidate;
				break;
			}
		}
	}
}

void myMesh::addVertexIfNotExists(std::vector<myVertex*>& nouveauxSommets, myVertex* vertex) {
	if (std::find(nouveauxSommets.begin(), nouveauxSommets.end(), vertex) == nouveauxSommets.end()) {
		nouveauxSommets.push_back(vertex);
	}
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

void myMesh::subdivisionCatmullClark()
{
	std::vector<myFace*> nouvellesFaces;
	std::vector<myHalfedge*> nouvellesAretes;
	std::vector<myVertex*> nouveauxSommets;

	std::map<myFace*, myVertex*> pointsFace;
	std::map<myHalfedge*, myVertex*> pointsArete;

	computeFacePoints(pointsFace);
	computeEdgeMidpoints(pointsArete, pointsFace);
	updateVertices(pointsFace);

	for (myFace* vieilleFace : faces) {
		createNewElements(vieilleFace, nouvellesFaces, nouvellesAretes, nouveauxSommets, pointsFace, pointsArete);
	}

	assignTwinsForNewEdges(nouvellesAretes);

	halfedges = nouvellesAretes;
	faces = nouvellesFaces;
	vertices = nouveauxSommets;

	checkFace();
	checkMesh();
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

bool myMesh::triangulate(myFace *f)
{
	/**** TODO ****/
	return false;

}

void myMesh::simplify() {
	myHalfedge* edgeToCollapse = nullptr;
	double minLength = std::numeric_limits<double>::infinity();

	for (myHalfedge* edge : halfedges) {
		if (!edge || !edge->next || !edge->twin) continue;

		myVertex* u = edge->source;
		myVertex* v = edge->next->source;

		if (!u || !v || !u->point || !v->point) continue;

		double len = u->point->dist(*v->point);
		if (len < minLength) {
			minLength = len;
			edgeToCollapse = edge;
		}
	}

	if (!edgeToCollapse || !edgeToCollapse->twin) return;

	myVertex* v1 = edgeToCollapse->source;
	myVertex* v2 = edgeToCollapse->next->source;

	myPoint3D* midpoint = new myPoint3D(
		(v1->point->X + v2->point->X) * 0.5,
		(v1->point->Y + v2->point->Y) * 0.5,
		(v1->point->Z + v2->point->Z) * 0.5
	);

	myVertex* mergedVertex = new myVertex();
	mergedVertex->point = midpoint;
	mergedVertex->normal = new myVector3D();
	vertices.push_back(mergedVertex);

	std::vector<myHalfedge*> outgoingFromV1, outgoingFromV2, toRemove;
	toRemove.push_back(edgeToCollapse);
	toRemove.push_back(edgeToCollapse->twin);

	for (myHalfedge* he : halfedges) {
		if (he->source == v1) outgoingFromV1.push_back(he);
		else if (he->source == v2) outgoingFromV2.push_back(he);
	}

	for (myHalfedge* he : outgoingFromV1)
		if (he != edgeToCollapse && he != edgeToCollapse->twin)
			he->source = mergedVertex;

	for (myHalfedge* he : outgoingFromV2)
		if (he != edgeToCollapse && he != edgeToCollapse->twin)
			he->source = mergedVertex;

	auto updateFace = [](myHalfedge* edge) {
		if (!edge || !edge->adjacent_face) return;
		myHalfedge* prev = edge->prev;
		myHalfedge* next = edge->next;
		if (prev && next) {
			prev->next = next;
			next->prev = prev;
			edge->adjacent_face->adjacent_halfedge = next;
		}
		};

	updateFace(edgeToCollapse);
	updateFace(edgeToCollapse->twin);

	std::vector<myFace*> degenerateFaces;
	for (myFace* face : faces) {
		if (!face->adjacent_halfedge) continue;

		int count = 0;
		myHalfedge* start = face->adjacent_halfedge;
		myHalfedge* current = start;
		do {
			count++;
			if (!current->next) break;
			current = current->next;
		} while (current != start);

		if (count < 3) {
			degenerateFaces.push_back(face);
			current = start;
			do {
				if (std::find(toRemove.begin(), toRemove.end(), current) == toRemove.end())
					toRemove.push_back(current);
				current = current->next;
			} while (current != start);
		}
	}

	for (myFace* f : degenerateFaces) {
		auto it = std::find(faces.begin(), faces.end(), f);
		if (it != faces.end()) {
			faces.erase(it);
			delete f;
		}
	}

	for (myHalfedge* he : toRemove) {
		auto it = std::find(halfedges.begin(), halfedges.end(), he);
		if (it != halfedges.end()) {
			halfedges.erase(it);
			delete he;
		}
	}

	auto removeVertex = [&](myVertex* v) {
		auto it = std::find(vertices.begin(), vertices.end(), v);
		if (it != vertices.end()) {
			vertices.erase(it);
			delete v;
		}
		};

	removeVertex(v1);
	removeVertex(v2);

	std::map<std::pair<myVertex*, myVertex*>, myHalfedge*> edgePairs;
	for (myHalfedge* he : halfedges) {
		if (!he->next) continue;

		myVertex* a = he->source;
		myVertex* b = he->next->source;

		auto key = std::make_pair(a, b);
		auto revKey = std::make_pair(b, a);

		if (edgePairs.count(revKey)) {
			he->twin = edgePairs[revKey];
			edgePairs[revKey]->twin = he;
		}
		else {
			edgePairs[key] = he;
		}
	}
}

bool myMesh::checkFace() {
	for (myFace* f : faces) {
		if (!f || !f->adjacent_halfedge) return false;
	}
	return true;
}





