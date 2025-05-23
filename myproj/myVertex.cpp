#include "myVertex.h"
#include "myvector3d.h"
#include "myHalfedge.h"
#include "myFace.h"

myVertex::myVertex(void)
{
	point = NULL;
	originof = NULL;
	normal = new myVector3D(1.0,1.0,1.0);
}

myVertex::~myVertex(void)
{
	if (normal) delete normal;
}

void myVertex::computeNormal()
{
    if (!originof || !normal) return;

    myVector3D sommeNormales(0.0, 0.0, 0.0);
    myHalfedge* courant = originof;
    myHalfedge* debut = courant;

    int compteur = 0;

    do {
        if (courant->adjacent_face && courant->adjacent_face->normal) {
            sommeNormales += *(courant->adjacent_face->normal);
            compteur++;
        }

        if (!courant->twin || !courant->twin->next) {
            break;
        }

        courant = courant->twin->next;

    } while (courant != debut);

    if (compteur > 0) {
        *normal = sommeNormales / compteur;
        normal->normalize();
    }
}

