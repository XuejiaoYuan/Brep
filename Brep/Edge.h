#pragma once
#include "HalfEdge.h"

class HalfEdge;

class Edge
{
public:
	Edge(HalfEdge* _lhe, HalfEdge* _rhe) :lhe(_lhe), rhe(_rhe) {}

	HalfEdge *lhe, *rhe;
};

