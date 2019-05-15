#ifndef _RENDERING_CONTROL_H_
#include "Rendering_Control.h"
#endif
//#include "stdafx.h"
Rendering_Control mrc(10, 10);
static int mainWindow;
static GLdouble camera[6] = { 50.0, 50.0, 100.0, 50.0, 50.0, 0.0 };
float strength = 80.0;
void initScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	Corn* newcorn1 = new Corn(0, 0, 0);
	Corn *newcorn2 = new Corn(1, 1, 1);
	Corn* newcorn3 = new Corn(3, 3, 2);
	//Bean *newbean = new Bean(4, 4, 2);
	Squash *newsquash1 = new Squash(8, 8, 0);
	Squash* newsquash2 = new Squash(5, 5, 1);
	Squash* newsquash3 = new Squash(8, 1, 2);
	mrc.Modify_Sun(1, strength, 0.0, 100.0, 80);
	mrc.Init();
	mrc.AddCrop(newcorn1);
	mrc.AddCrop(newcorn2);
	mrc.AddCrop(newcorn3);
	//mrc.AddCrop(newbean);
	mrc.AddCrop(newsquash1);
	mrc.AddCrop(newsquash2);
	mrc.AddCrop(newsquash3);
	//mrc.DelCrop(2);
	mrc.AddWater(0, 0, 2);
	mrc.AddWater(1, 1, 1);
}
void keyboard(unsigned char key, int x, int y) { 
	switch (key) 
	{ 
	case 'o':
		strength--;
		mrc.Modify_Sun(1, strength, 0.0, 100.0, 80);
		break;
	case 'p':
		strength++;
		mrc.Modify_Sun(1, strength, 0.0, 100.0, 80);
		break;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
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
	glewInit();
	initScene();
	glutDisplayFunc(Rendering);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}