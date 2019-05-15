#include "Rendering_Control.h"

Rendering_Control::Rendering_Control(int length, int width):grids(length, width) {}

void Rendering_Control::Init() {
	weather.Init();
}

void Rendering_Control::AddCrop(Plants* plant) {
	plant->modifyPos(plant->getX() * (grids.grid_height + 0), plant->getY() * (grids.grid_height + 0));
	Crops.push_back(plant);
}

void Rendering_Control::DelCrop(int index) {
	delete *(Crops.begin() + index);
	Crops.erase(Crops.begin() + index);
}

void Rendering_Control::AddWater(int x, int y, int state) {
	grids.ChangeState(x, y, state);
}

void Rendering_Control::Modify_Sun(int state, float dir_x, float dir_y, float dir_z, int strength) {
	weather.modify_Sun(state, dir_x, dir_y, dir_z, strength);
}

void Rendering_Control::Modify_Rain(int state, float dir_x, float dir_y, float dir_z, int strength) {
	weather.modify_Rain(state, dir_x, dir_y, dir_z, strength);
}

void Rendering_Control::Rendering(int debug) {
	for (auto t : Crops) {
		if (t->Type() == "Corn") {
			((Corn*)t)->Rendering();
		}
		else if (t->Type() == "Squash") {
			((Squash*)t)->Rendering();
		}
		else if (t->Type() == "Bean") {
			((Bean*)t)->Rendering();
		}
	}
	weather.Rendering();
	grids.Rendering();
}