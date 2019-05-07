#ifndef _RENDERING_PLANTS_H_
#define _RENDERING_PLANTS_H_
#ifndef _STRING_
#include<string>
#endif
#ifndef __glut_h__
#include <GL/gl.h>
#include <GL/glut.h>
// #include "glut\glut.h"
#endif
class Plants {
private:
	std::string Plant_Type;
	int Plant_Pos_X, Plant_Pos_Y;
	int Plant_Maturity;
public:
	Plants(std::string Plant_Type, int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	int getX() const { return Plant_Pos_X; }
	int getY() const { return Plant_Pos_Y; }
	void modifyX(int x);
	void modifyY(int y);
	int Maturity() const { return Plant_Maturity; }
	std::string Type() const { return Plant_Type; }
};

class Corn : public Plants {
private:
	GLUquadric *pObj;
public:
	Corn(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	void Rendering(GLdouble  baseRadius, GLdouble  topRadius, GLdouble  height, GLint  slices, GLint  stacks);
	~Corn();
};

class Squash : public Plants {
private:
	GLUquadric *pObj;
public:
	Squash(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	void Rendering(GLdouble radius, GLint  slices, GLint stacks);
	~Squash();
};

class Bean : public Plants {
private:
	GLUquadric *pObj;
public:
	Bean(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	void Rendering(GLdouble radius, GLdouble height, GLint  slices, GLint stacks);
	~Bean();
};
#endif // !RENDERING_H


