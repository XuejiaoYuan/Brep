#include "EulerOperator.h"

EulerOperator::EulerOperator()
{
}

void EulerOperator::initialObject(vector<vector<double>>& coordinates)
{
	solid = mvsf(coordinates[0]);
	Loop* newLoop = solid->faceList[0]->loopList[0];
	for (int i = 1; i < coordinates.size(); i++) {
		Vertex* endv = new Vertex(coordinates[i], solid->vertexList.size());
		mev(solid->vertexList[solid->vertexList.size() - 1], endv, newLoop);
	}
	mef(solid->vertexList[solid->vertexList.size() - 1], solid->vertexList[0], newLoop);
}

void EulerOperator::addInnerLoops(vector<vector<double>>& coordinates)
{
	Loop* curLoop = solid->faceList[1]->loopList[0];	//最大区域对应的环
	vector<Vertex*>& solidVertexList = solid->vertexList;
	Vertex* v = new Vertex(coordinates[0], solid->vertexList.size());
	HalfEdge* he = mev(solid->vertexList[0], v, curLoop);
	Loop* lp = kemr(he);
	for (int i = 1; i < coordinates.size(); i++) {
		Vertex* v = new Vertex(coordinates[i], solid->vertexList.size());
		mev(solidVertexList[solidVertexList.size() - 1], v, lp);
	}

	int startPos = solidVertexList.size() - coordinates.size();
	Loop* newLoop = mef(solidVertexList[solidVertexList.size() - 1], solidVertexList[startPos], lp);
	kfmrh(curLoop->lface, newLoop->lface);
}

void EulerOperator::sweeping(vector<double>& direction, double d)
{
	Loop* curLoop;
	HalfEdge* he;
	Vertex* firstv, *nextv, *firstup, *nextup, *prevup;
	Face* curFace = solid->faceList[0];

	for (int j = 0; j < solid->faceList[1]->loopList.size(); j++) {
		curLoop = solid->faceList[1]->loopList[j]->ledg->brotherEdg->wloop;
		he = solid->faceList[1]->loopList[j]->ledg;
		firstv = he->endv;
		nextv = he->startv;
		firstup = new Vertex(firstv->coordinate, direction, d, solid->vertexList.size());
		prevup = firstup;
		mev(firstv, firstup, curLoop);
		he = he->prv;
		while (nextv != firstv) {
			nextup = new Vertex(nextv->coordinate, direction, d, solid->vertexList.size());
			mev(nextv, nextup, curLoop);
			mef(nextup, prevup, curLoop);
			prevup = nextup;
			nextv = he->startv;
			he = he->prv;
		}
		Loop* newLoop = mef(prevup, firstup, curLoop);
		if (j == 0) {
			curFace = newLoop->lface;
		}
		else {
			kfmrh(curFace, newLoop->lface);
		}
	}

}

void EulerOperator::getBrpFile()
{
	fstream outFile("cube.brp", ios_base::out);
	outFile << "BRP" << endl;
	outFile << solid->vertexList.size() << " " << solid->lpNum << " " << solid->faceList.size() << " 1" << endl;
	for (auto v : solid->vertexList) {
		outFile << v->coordinate[0] << ' ' << v->coordinate[1] << ' ' << v->coordinate[2] << endl;
	}

	for (int i = 0; i < solid->faceList.size(); i++) {
		for (int j = 0; j < solid->faceList[i]->loopList.size(); j++) {
			Loop* curLoop = solid->faceList[i]->loopList[j];
			HalfEdge* he = curLoop->ledg;
			vector<int> idList;
			idList.push_back(he->startv->id);
			he = he->nxt;
			while (he != curLoop->ledg) {
				idList.push_back(he->startv->id);
				he = he->nxt;
			}
			outFile << idList.size();
			for (auto x : idList)
				outFile << " " << x;
			outFile << endl;
		}
	}

	for (auto faceList : solid->faceList) {
		outFile << faceList->loopList[0]->id << " " << faceList->loopList.size() - 1;
		for (int i = 1; i < faceList->loopList.size(); i++) {
			outFile << " " << faceList->loopList[i]->id;
		}
		outFile << " 0" << endl;
	}
}

EulerOperator::~EulerOperator()
{
}

Solid * EulerOperator::mvsf(const vector<double>& _coordinate)
{
	Solid* solid = new Solid();
	Face* face = new Face(solid, solid->faceList.size());
	Loop *lp = new Loop(face, solid->lpNum++);
	Vertex *v = new Vertex(_coordinate, solid->vertexList.size());

	solid->faceList.push_back(face);
	solid->vertexList.push_back(v);
	face->fsolid = solid;
	face->loopList.push_back(lp);

	return solid;	
}

HalfEdge * EulerOperator::mev(Vertex * startv, Vertex* endv, Loop * lp)
{
	Solid* curSolid = lp->lface->fsolid;
	solid->vertexList.push_back(endv);

	HalfEdge* lhe = new HalfEdge(startv, endv, lp);
	HalfEdge* rhe = new HalfEdge(endv, startv, lp);

	Edge* edg = new Edge(lhe, rhe);

	lhe->edg = edg;
	rhe->edg = edg;
	lhe->nxt = rhe;
	rhe->prv = lhe;
	lhe->brotherEdg = rhe;
	rhe->brotherEdg = lhe;

	if (lp->ledg == nullptr) {
		lhe->prv = rhe;
		rhe->nxt = lhe;
		lp->ledg = lhe;
	}
	else {
		HalfEdge* he = lp->ledg;
		for (; he->endv != startv; he = he->nxt);
		rhe->nxt = he->nxt;
		he->nxt->prv = rhe;
		he->nxt = lhe;
		lhe->prv = he;
	}

	return lhe;
}

Loop * EulerOperator::mef(Vertex * startv, Vertex * endv, Loop * lp)
{
	Solid *curSolid = lp->lface->fsolid;
	Face *newFace = new Face(curSolid, solid->faceList.size());
	Loop* newLoop = new Loop(newFace, curSolid->lpNum++);

	HalfEdge *lhe = new HalfEdge(startv, endv, newLoop);
	HalfEdge *rhe = new HalfEdge(endv, startv, lp);
	Edge* edg = new Edge(lhe, rhe);
	lhe->edg = edg;
	rhe->edg = edg;
	lhe->brotherEdg = rhe;
	rhe->brotherEdg = lhe;

	HalfEdge *tmphe1 = lp->ledg;
	HalfEdge *tmphe2 = lp->ledg;
	for (; tmphe1->startv != endv; tmphe1 = tmphe1->nxt);
	for (; tmphe2->endv != startv; tmphe2 = tmphe2->nxt);
	lhe->nxt = tmphe1;
	lhe->prv = tmphe2;
	rhe->nxt = tmphe2->nxt;
	rhe->prv = tmphe1->prv;
	tmphe1->prv = lhe;
	tmphe2->nxt = lhe;
	rhe->prv->nxt = rhe;
	rhe->nxt->prv = rhe;

	newLoop->ledg = lhe;
	for (tmphe1 = lhe; tmphe1->nxt != lhe; tmphe1 = tmphe1->nxt)
		tmphe1->wloop = newLoop;
	lp->ledg = rhe;

	//将面加入体中
	curSolid->faceList.push_back(newFace);

	//将环加入面中
	newFace->loopList.push_back(newLoop);

	return newLoop;
}

Loop * EulerOperator::kemr(HalfEdge* lhe)	
{
	Solid* curSolid = lhe->wloop->lface->fsolid;
	Edge* curEdg = lhe->edg;
	Face* newFace = new Face(curSolid, curSolid->faceList.size());
	HalfEdge* rhe = curEdg->rhe;
	Loop* newLoop = new Loop(newFace, solid->lpNum++);
	
	rhe->nxt->prv = lhe->prv;
	lhe->prv->nxt = rhe->nxt;

	curSolid->faceList.push_back(newFace);
	newFace->loopList.push_back(newLoop);

	delete curEdg;
	return newLoop;
}

void EulerOperator::kfmrh(Face * f1, Face * f2)
{
	f1->loopList.insert(f1->loopList.end(), f2->loopList.begin(), f2->loopList.end());
	
	for (auto it = f1->fsolid->faceList.begin(); it != f1->fsolid->faceList.end(); it++) {
		if (*it == f2) {
			f1->fsolid->faceList.erase(it);
			break;
		}
	}

	
	delete f2;
}

