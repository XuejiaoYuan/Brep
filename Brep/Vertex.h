#pragma once
#include "HalfEdge.h"
#include <vector>
using namespace std;

class HalfEdge;

class Vertex
{
public:
	Vertex(const vector<double>& _coordinate, int _id = -1) {
		coordinate = _coordinate;
		id = _id;
	}
	Vertex(const vector<double>& _coordinate, const vector<double>& direction, double d, int _id = -1) {
		coordinate.push_back(_coordinate[0] + direction[0] * d);
		coordinate.push_back(_coordinate[1] + direction[1] * d);
		coordinate.push_back(_coordinate[2] + direction[2] * d);
		id = _id;
	}
	vector<double> coordinate;
	int id;
};
