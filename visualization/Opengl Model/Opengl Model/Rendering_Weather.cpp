#include"Rendering_Weather.h"

Weather::Weather() {
	Sun_State = 1;
	Sun_Direction[0] = 0.0;
	Sun_Direction[1] = 0.0;
	Sun_Direction[2] = 1.0; 
	Sun_Strength = 1.0;
	Rain_State = 0;
	Rain_Direction[0] = 0.0;
	Rain_Direction[1] = 0.0;
	Rain_Direction[2] = 1.0;
	Rain_Strength = 1;
}

void Weather::modify_Sun(int state, float dir_x, float dir_y, float dir_z, int strength) {
	Sun_State = state;
	Sun_Direction[0] = dir_x;
	Sun_Direction[1] = dir_y;
	Sun_Direction[2] = dir_z;
	Sun_Strength = strength;
}

void Weather::modify_Rain(int state, float dir_x, float dir_y, float dir_z, int strength) {
	Rain_State = state;
	Rain_Direction[0] = dir_x;
	Rain_Direction[1] = dir_y;
	Rain_Direction[2] = dir_z;
	Rain_Strength = strength;
}

void Weather::Init() {
	GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 80.0f };
	GLfloat light_position[] = { 0.0f, 0.0f, 100.0f, 0.0f };
	GLfloat light_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light_Model_Ambient[] = { 0.5f , 0.5f , 0.5f , 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void Weather::Rendering() {
	if (Sun_State == 1) {
		glLightfv(GL_LIGHT0, GL_POSITION, Sun_Direction);

		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LIGHTING);
		glDisable(GL_BLEND);
	}
}