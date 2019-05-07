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
	colors[0] = new float[3]{ (float)0.1,(float)0.05,0 };
	colors[1] = new float[3]{ (float)0.3,(float)0.15,0 };
	colors[2] = new float[3]{ (float)0.5,(float)0.25,0 };
}
void Grids::RenderingOne(int x, int y) {
	glPushMatrix();
	glColor3f(colors[grid_state[x][y]][0], colors[grid_state[x][y]][1], colors[grid_state[x][y]][2]);
	glTranslatef((grid_height + 0) * x, (grid_height + 0) * y, 0);
	glScalef(grid_height, grid_height, 1.0);
	glutSolidCube(1.0);
	glColor3f(0.0, 1.0, 0.0);
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