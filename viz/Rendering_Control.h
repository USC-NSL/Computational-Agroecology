#ifndef _RENDERING_CONTROL_H_
#define _RENDERING_CONTROL_H_

#ifndef _RENDERING_PLANTS_H_
#include "Rendering_Plants.h"
#endif
#ifndef _RENDERING_GRIDS_H_
#include "Rendering_Grids.h"
#endif
#ifndef _RENDERING_WEATHER_H_
#include "Rendering_Weather.h"
#endif
#ifndef _VECTOR_
#include <vector>
#endif
class Rendering_Control {
private:
	Grids grids;
	Weather weather;
	std::vector<Plants*> Crops;
public:
	Rendering_Control(int length, int width);
	void Init();
	void AddCrop(Plants* plant);
	void DelCrop(int index);
	void AddWater(int x, int y, int state);
	void Modify_Sun(int state, float dir_x, float dir_y, float dir_z, int strength);
	void Modify_Rain(int state, float dir_x, float dir_y, float dir_z, int strength);
	void Rendering(int debug=0);
};
#endif // !_RENDERING_CONTROL_H_

