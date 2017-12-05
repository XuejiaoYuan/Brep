#pragma once
#include "Solid.h"
#include "Loop.h"
#include <vector>
using namespace std;

class Solid;
class Loop;

class Face
{
public:
	Face(Solid* _solid = nullptr, int _id = -1) :
		fsolid(_solid), id(_id){}

	Solid *fsolid;
	vector<Loop*> loopList;
	int id;
};


