
#include "Rendering_Control.h"
#ifndef __glut_h__
#include "glut\glut.h"
#endif
#pragma comment(lib, "glut32.lib")
Rendering_Control mrc(10, 10);
static int mainWindow;
static GLdouble camera[6] = { 0.0, 0.0, 50.0, 0.0, 0.0, 0.0 };
void initScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	Corn *newcorn = new Corn(1, 1, 0);
	Bean *newbean = new Bean(4, 4, 2);
	Squash *newsquash = new Squash(8, 8, 3);
	mrc.AddCrop(newcorn);
	mrc.AddCrop(newbean);
	mrc.AddCrop(newsquash);
	//mrc.DelCrop(2);
	mrc.AddWater(0, 0);
}
void keyboard(unsigned char key, int x, int y) { 
	switch (key) 
	{ 
	case 'd':
		camera[0] += 1.0;
		break;
	case 'a':
		camera[0] -= 1.0;
		break;
	case 'w':
		camera[1] += 1.0;
		break;
	case 's':
		camera[1] -= 1.0;
		break;
	case 'z':
		camera[2] += 1.0;
		break;
	case 'x':
		camera[2] -= 1.0;
		break;
	case '1':
		camera[3] += 1.0;
		break;
	case '2':
		camera[3] -= 1.0;
		break;
	case '3':
		camera[4] += 1.0;
		break;
	case '4':
		camera[4] -= 1.0;
		break;
	case '5':
		camera[5] += 1.0;
		break;
	case '6':
		camera[5] -= 1.0;
		break;
	case 27:
		exit(0);
		break; 
	}
	glutSetWindow(mainWindow);
	glutPostRedisplay();
}
void Rendering(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity(); 
	gluLookAt(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], 1.0, 0.0, 0.0);
	mrc.Rendering();
	glFlush();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 3000.0);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	mainWindow = glutCreateWindow("VIZ");
	initScene();
	glutDisplayFunc(Rendering);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}