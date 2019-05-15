#include "Rendering_Grids.h"
Grids::Grids(int length, int width) {
	grid_length = length;
	grid_width = width;
	grid_state = new int*[length];
	for (int i = 0; i < length; i++) {
		grid_state[i] = new int[width];
		for (int j = 0; j < width; j++) {
			grid_state[i][j] = 0;
		}
	}
	colors = new float*[3];
	colors[0] = new float[4]{ 0.1f,0.05f,0.02f, 1.0f };
	colors[1] = new float[4]{ 0.2f,0.15f,0.01f, 1.0f };
	colors[2] = new float[4]{ 0.3f,0.25f,0.01f, 1.0f };
}
void Grids::RenderingOne(int x, int y) {
	glPushMatrix();
	glTranslatef((grid_height + 0) * x, (grid_height + 0) * y, 0);
	GLfloat mat_ambient[] = { 0.0f,	1.0f, 0.0f, 1.0f };
	GLfloat mat_diffuse[] = { 0.0f,	1.0f, 0.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 80.0f };
	glScalef(grid_height, grid_height, 1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[grid_state[x][y]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[grid_state[x][y]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glutSolidCube(1.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glLineWidth(2.0);
	glutWireCube(1.0);
	glPopMatrix();
}

void Grids::ChangeState(int x, int y, int state) {
	grid_state[x][y] = state;
}

void Grids::ChangeStates(int ** states) {
	for (int i = 0; i < grid_length; i++) {
		for (int j = 0; j < grid_width; j++) {
			grid_state[i][j] = states[i][j];
		}
	}
}

void Grids::Rendering() {
	for (int i = 0; i < grid_length; i++) {
		for (int j = 0; j < grid_width; j++) {
			RenderingOne(i, j);
		}
	}
}

Grids::~Grids() {
	delete[] grid_state;
	delete colors;
}