#pragma once
#include "Loop.h"
#include "Edge.h"
#include "Vertex.h"

class Loop;
class Edge;
class Vertex;

class HalfEdge
{
public:
	HalfEdge(Vertex* _stv, Vertex *_endv, Loop* _lp = nullptr) :startv(_stv), endv(_endv), wloop(_lp),
		nxt(nullptr), prv(nullptr), brotherEdg(nullptr), edg(nullptr) {}

	HalfEdge *nxt, *prv;
	HalfEdge *brotherEdg;
	Edge *edg;
	Loop *wloop;
	Vertex *startv, *endv;
};

