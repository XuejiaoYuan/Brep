#pragma once
#include "Face.h"
#include "HalfEdge.h"

class Face;
class HalfEdge;

class Loop
{
public:
	Loop(Face* _face = nullptr, int _id = -1, HalfEdge* _ledge = nullptr) :
		lface(_face), ledg(_ledge), id(_id) {}

	Face *lface;
	HalfEdge *ledg;
	int id;
};

