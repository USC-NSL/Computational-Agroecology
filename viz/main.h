#ifndef __MAIN_H__
#define __MAIN_H__
#include "common/stdafx.h"
#include "map_control.h"
#include "model/model.h"

static int mainWindow;
static GLdouble camera[9] = {0.0, 0.0, 2.0,
                             1.0, 1.0, 1.0,
                             0.0, 1.0, 0.0};
std::vector<Model> models;
map_control map(10, 10, Vector3(0.0, 0.0, -1.0), 255);

int scrn_width = 800;
int scrn_height = 600;
int scrn_pos_x = 50;
int scrn_pos_y = 50;

void PhotonMapping(void);

#endif /* __MAIN_H__ */
