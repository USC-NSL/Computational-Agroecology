#include "Rendering_Plants.h"
Plants::Plants(std::string Plant_Type, int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity) {
	this->Plant_Type = Plant_Type;
	this->Plant_Pos_X = Plant_Pos_X;
	this->Plant_Pos_Y = Plant_Pos_Y;
	this->Plant_Maturity = Plant_Maturity;
}


void Plants::modifyX(int x) {
	Plant_Pos_X = x;
}

void Plants::modifyY(int y) {
	Plant_Pos_Y = y;
}

Corn::Corn(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity): Plants("Corn", Plant_Pos_X, Plant_Pos_Y, Plant_Maturity){
	pObj = gluNewQuadric();
}

void Corn::Rendering(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks) {
	glPushMatrix();
	glColor3f(1.0, 1.0, 0);
	glTranslatef(getX(), getY(), 0.0);	
	gluCylinder(pObj, baseRadius, topRadius, height, slices, stacks);
	glPopMatrix();
}

Corn::~Corn() {
	gluDeleteQuadric(pObj);
}


Squash::Squash(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity) : Plants("Squash", Plant_Pos_X, Plant_Pos_Y, Plant_Maturity) {
	pObj = gluNewQuadric();
}

void Squash::Rendering(GLdouble radius, GLint slices, GLint stacks) {
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(getX(), getY(), 0.0);
	gluSphere(pObj, radius, slices, stacks);	
	glPopMatrix();
}

Squash::~Squash() {
	gluDeleteQuadric(pObj);
}


Bean::Bean(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity) : Plants("Bean", Plant_Pos_X, Plant_Pos_Y, Plant_Maturity) {
	pObj = gluNewQuadric();
}

void Bean::Rendering(GLdouble radius, GLdouble height, GLint slices, GLint stacks) {
	glPushMatrix();
	glColor3f(0.0, 0.5, 0.6);
	glTranslatef(getX(), getY(), 0.0);
	gluCylinder(pObj, 0, radius, height, slices, stacks);
	glPopMatrix();
}

Bean::~Bean() {
	gluDeleteQuadric(pObj);
}