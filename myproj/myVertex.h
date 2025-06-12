#pragma once
#include "mypoint3d.h"
#include <vector>

class myHalfedge;
class myVector3D;

class myVertex
{
public:
	myPoint3D *point;
	myHalfedge *originof;

	int index;  //use as you wish.

	myVector3D *normal;

	std::vector<myHalfedge*> outgoing_halfedges;
	bool isNew = false;

	void computeNormal();
	myVertex(void);
	~myVertex(void);
};
