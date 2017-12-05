#pragma once
#include "Edge.h"
#include "Face.h"
#include "Vertex.h"
#include <vector>
using namespace std;

class Edge;
class Face;
class Vertex;

class Solid
{
public:
	Solid() :nexts(nullptr), prevs(nullptr), lpNum(0) {}
	
	Solid *nexts, *prevs;
	int lpNum;
	vector<Face*> faceList;
	vector<Vertex*> vertexList;
};

