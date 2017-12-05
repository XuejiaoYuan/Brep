#include "EulerOperator.h"
#include <iostream>
#include <string>
#include <sstream>
#include <GL/glut.h>
using namespace std;
#pragma comment(lib, "glut32.lib")

GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };	//环境光
GLfloat lightDiffuse[4] = { 0.39, 1.0, 0.0, 1.0 };	//漫反射光
GLfloat lightPosition[4] = { 0.0, 0.0, 20, 1.0 };	//光源位置，通常将屏幕位置设为z轴的0.0点

vector<vector<double*>> sideVertex;
vector<vector<vector<double*>>> tdVertex;

void CALLBACK vertexCallback(GLvoid *vertex)
{
	const GLdouble *pointer = (GLdouble *)vertex;
	//glColor3dv(pointer + 3);//在此设置颜色  

	double color[3] = { 0.39, 1, 0 };
	glColor3dv(color);
	glVertex3dv(pointer);
	glNormal3dv(pointer + 3);
	cout << "vertex:" << pointer[0] << ' ' << pointer[1] << ' ' << pointer[2] << endl;
	cout << "normal:" << pointer[3] << ' ' << pointer[4] << ' ' << pointer[5] << endl;
}
void CALLBACK beginCallback(GLenum which)
{
	glBegin(which);
}
void CALLBACK endCallback()
{
	glEnd();
}
void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}
void CALLBACK combineCallback(GLdouble coords[3],
	GLdouble *vertex_data[4],
	GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;
	int i;
	vertex = (GLdouble *)malloc(6 * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	for (i = 3; i < 7; i++)
	{
		vertex[i] = weight[0] * vertex_data[0][i]
			+ weight[1] * vertex_data[1][i]
			+ weight[2] * vertex_data[2][i]
			+ weight[3] * vertex_data[3][i];
	}
	*dataOut = vertex;
}

void OnDraw()
{

	// clear the screen & depth buffer  
	glClear(GL_COLOR_BUFFER_BIT);

	// clear the previous transform  
	glLoadIdentity();

	GLUtesselator *tobj = gluNewTess();
	if (!tobj) { return; }

	gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
	//gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);

	// glShadeModel(GL_FLAT);  

	// gluTessProperty(tobj,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_POSITIVE); //GLU_TESS_WINDING_ODD  

	for (int k = 0; k < tdVertex.size(); k++) {
		gluTessBeginPolygon(tobj, NULL);
		for (int i = 0; i < tdVertex[k].size(); i++)
		{
			gluTessBeginContour(tobj);
			for (int j = 0; j < tdVertex[k][i].size(); j++) {
				if (k == 0) {
					tdVertex[k][i][j][3] = 0;
					tdVertex[k][i][j][4] = 0;
					tdVertex[k][i][j][5] = -1;
				}
				else {
					tdVertex[k][i][j][3] = 0;
					tdVertex[k][i][j][4] = 0;
					tdVertex[k][i][j][5] = 1;
				}
				gluTessVertex(tobj, tdVertex[k][i][j], tdVertex[k][i][j]);
			}
			gluTessEndContour(tobj);
		}
		gluTessEndPolygon(tobj);
	}


	gluDeleteTess(tobj);

	glutSwapBuffers();

}


void OnInit()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);	//启用深度测试

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	//设置环境光的发光亮，
	//光源GL_LIGHT1开始发光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	//设置漫反射发光量
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	//设置光源的位置
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);		//启用一号光源
}

void OnReshape(int w, int h)
{
	// prevents division by zero when minimising window  
	if (h == 0)
	{
		h = 1;
	}

	// set the drawable region of the window  
	glViewport(0, 0, w, h);

	// set up the projection matrix  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection  
	gluPerspective(45,(float)w/h,0.1,100);  
	//if (w <= h)
	//{
	//	glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w, 4.0 * (GLfloat)h / (GLfloat)w, 0.0, 100.0);
	//}
	//else
	//{
	//	glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w, 4.0 * (GLfloat)h / (GLfloat)w, 0.0, 100.0);
	//}

	// go back to model view matrix so we can move the objects about  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void File2Points(string FileName, vector<vector<double>>& outVertexList, 
	vector<vector<vector<double>>>& inVertexList, vector<double>& direction, double& d) 
{
	ifstream LoopFile(FileName);
	string s;
	istringstream ss;
	vector<double> v(3,0);
	int cnt;

	getline(LoopFile, s);
	if (s == "#outterLoop") {
		while (true) {
			getline(LoopFile, s);
			if (s == "#innerLoop")
				break;
			ss.clear();
			ss.str(s);
			ss >> v[0] >> v[1] >> v[2];
			outVertexList.push_back(v);
		}
	}
	if (s == "#innerLoop") {
		while (!LoopFile.eof()) {
			getline(LoopFile, s);
			if (s == "#direction") {
				getline(LoopFile, s);
				ss.clear();
				ss.str(s);
				ss >> direction[0] >> direction[1] >> direction[2] >> d;
				break;
			}
			ss.clear();
			ss.str(s);
			ss >> cnt;
			vector<vector<double>> vList;
			for (int i = 0; i < cnt; i++) {
				getline(LoopFile, s);
				ss.clear();
				ss.str(s);
				ss >> v[0] >> v[1] >> v[2];
				vList.push_back(v);
			}
			inVertexList.push_back(vList);
		}
	}

}

void Points2File(Solid* solid) {
	ofstream outFile("testCube.brp");
	outFile << "BRP" << endl;
	outFile << solid->vertexList.size() << ' ' << solid->lpNum << ' ' << solid->faceList.size() << " 1" << endl;
	
	vector<vector<double>> outVertex(solid->vertexList.size(), vector<double>(3, 0));
	for (int i = 0; i < solid->vertexList.size(); i++) {
		outVertex[solid->vertexList[i]->id][0] = solid->vertexList[i]->coordinate[0];
		outVertex[solid->vertexList[i]->id][1] = solid->vertexList[i]->coordinate[1];
		outVertex[solid->vertexList[i]->id][2] = solid->vertexList[i]->coordinate[2];
	}

	vector<vector<int>> outLoop(solid->lpNum);
	for (int i = 0; i < solid->faceList.size(); i++) {
		for (int j = 0; j < solid->faceList[i]->loopList.size(); j++) {
			int loopId = solid->faceList[i]->loopList[j]->id;
			HalfEdge*he = solid->faceList[i]->loopList[j]->ledg;
			Vertex* v = he->startv;
			Vertex* firstv = v;
			do
			{
				outLoop[loopId].push_back(v->id);
				he = he->nxt;
				v = he->startv;
			} while (v != firstv);
		}
	}

	vector<vector<int>> outFace(solid->faceList.size());
	for (int i = 0; i < solid->faceList.size(); i++) {
		outFace[i].push_back(solid->faceList[i]->loopList[0]->id);
		int inLoopSize = solid->faceList[i]->loopList.size() - 1;
		if (inLoopSize > 0) {
			outFace[i].push_back(inLoopSize);
			for (int j = 1; j <= inLoopSize; j++) {
				outFace[i].push_back(solid->faceList[i]->loopList[j]->id);
			}
		}
		else
			outFace[i].push_back(0);
		outFace[i].push_back(0);
	}

	for (auto v : outVertex)
		outFile << v[0] << ' ' << v[1] << ' ' << v[2] << endl;

	for (auto loop : outLoop) {
		outFile << loop.size();
		for (auto v : loop)
			outFile << ' ' << v;
		outFile << endl;
	}

	for (int i = 0; i < outFace.size(); i++) {
		outFile << outFace[i][0];
		for (int j = 1; j < outFace[i].size(); j++) {
			outFile << " " << outFace[i][j];
		}
		outFile << endl;
	}

}

void Solid2Points(Solid *solid)
{
	HalfEdge *he;
	Vertex *v, *firstv;
	double *cord;
	for (int i = 0; i<solid->faceList.size(); i++) {
		vector<vector<double*>> fvList;
		int LoopSize = solid->faceList[i]->loopList.size();
		for (int j = 0; j<LoopSize; j++) {
			vector<double*> lvList;
			he = solid->faceList[i]->loopList[j]->ledg;
			v = he->startv;
			firstv = v;
			do {
				cord = new  double[3];
				cord[0] = v->coordinate[0];
				cord[1] = v->coordinate[1];
				cord[2] = v->coordinate[2];
				lvList.push_back(cord);
				he = he->nxt;
				v = he->startv;
			} while (v != firstv);
			if (LoopSize == 1) {
				sideVertex.push_back(lvList);
			}
			else {
				fvList.push_back(lvList);
			}
		}
		if (!fvList.empty())
			tdVertex.push_back(fvList);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: [LoopFileName]";
		return -1;
	}
	string LoopFileName = string(argv[1]);
	vector<vector<double>> outVertexList;
	vector<vector<vector<double>> > inVertexList;
	vector<double> direction(3, 0);
	double d;

	File2Points(LoopFileName, outVertexList, inVertexList, direction, d);

	EulerOperator euler;
	euler.initialObject(outVertexList);

	for (int i = 0; i < inVertexList.size(); i++)
		euler.addInnerLoops(inVertexList[i]);

	euler.sweeping(direction, d);

	Points2File(euler.getSolid());
	//Solid2Points(euler.getSolid());
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	//glutInitWindowSize(600, 400);
	//glutCreateWindow("Euler operator");
	//OnInit();
	//glutDisplayFunc(OnDraw);
	//glutReshapeFunc(OnReshape);
	//glutMainLoop();


	return 0;
}