#ifndef _RENDERING_WEATHER_H_
#define _RENDERING_WEATHER_H_
#include "stdafx.h"
class Weather {
private:
	int Sun_State;
	float Sun_Direction[3];
	int Sun_Strength;
	int Rain_State;
	float Rain_Direction[3];
	int Rain_Strength;
public:
	Weather();
	void Init();
	void modify_Sun(int state, float dir_x, float dir_y, float dir_z, int strength);
	void modify_Rain(int state, float dir_x, float dir_y, float dir_z, int strength);
	void Rendering();
};
#endif // !_RENDERING_WEATHER_H_