#pragma once
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Loop.h"
#include "Solid.h"
#include "Vertex.h"
#include <vector>
#include <fstream>
using namespace std;

class EulerOperator
{
public:
	EulerOperator();
	void initialObject(vector<vector<double>> &coordinates);
	void addInnerLoops(vector<vector<double>> &coordinates);
	void sweeping(vector<double>& direction, double d);
	Solid* getSolid() { return solid; }

	//for test
	void getBrpFile();

	~EulerOperator();

private:
	Solid *solid;
	Solid* mvsf(const vector<double>& _coordinate);
	HalfEdge* mev(Vertex* startv, Vertex* endv, Loop* lp);
	Loop* mef(Vertex* startv, Vertex *endv, Loop *lp);
	Loop* kemr(HalfEdge* rhe);
	void kfmrh(Face* f1, Face* f2);
};

