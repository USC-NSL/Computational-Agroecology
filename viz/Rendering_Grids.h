#ifndef _RENDERING_GRIDS_H_
#define _RENDERING_GRIDS_H_
#include "stdafx.h"
class Grids {
private:
	int grid_length;
	int grid_width;
	int **grid_state;
	void RenderingOne(int x, int y);
	float **colors;
public:
	Grids(int length, int width);
	const int grid_height = 5;
	void ChangeState(int x, int y, int state);
	void ChangeStates(int ** states);
	void Rendering();
	~Grids();
};
#endif // !_RENDERING_GRIDS_H_

