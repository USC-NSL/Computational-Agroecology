#ifndef _RENDERING_CONTROL_H_
#define _RENDERING_CONTROL_H_
#ifndef _VECTOR_
#include <vector>
#endif
#ifndef _RENDERING_PLANTS_H_
#include "Rendering_Plants.h"
#endif
#ifndef _RENDERING_GRIDS_H_
#include "Rendering_Grids.h"
#endif
class Rendering_Control {
private:
	Grids grids;
	std::vector<Plants*> Crops;
public:
	Rendering_Control(int length, int width);
	void AddCrop(Plants* plant);
	void DelCrop(int index);
	void AddWater(int x, int y);
	void Rendering(int debug=0);
};
#endif // !_RENDERING_CONTROL_H_

