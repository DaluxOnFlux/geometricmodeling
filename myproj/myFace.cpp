#include "myFace.h"
#include "myvector3d.h"
#include "myHalfedge.h"
#include "myVertex.h"
#include <GL/glew.h>

myFace::myFace(void)
{
	adjacent_halfedge = NULL;
	normal = new myVector3D(1.0, 1.0, 1.0);
}

myFace::~myFace(void)
{
	if (normal) delete normal;
}

void myFace::computeNormal()
{
    myHalfedge* he1 = adjacent_halfedge;
    myHalfedge* he2 = he1->next;
    myHalfedge* he3 = he2->next;

    myVector3D edge1(
        he2->source->point->X - he1->source->point->X,
        he2->source->point->Y - he1->source->point->Y,
        he2->source->point->Z - he1->source->point->Z);

    myVector3D edge2(
        he3->source->point->X - he2->source->point->X,
        he3->source->point->Y - he2->source->point->Y,
        he3->source->point->Z - he2->source->point->Z);

    *normal = edge1.crossproduct(edge2);
    normal->normalize();
}

