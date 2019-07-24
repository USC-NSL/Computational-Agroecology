#ifndef __MAP_CONTROL_H__
#define __MAP_CONTROL_H__
#ifndef _PHOTON_CONTROL_H_
#include "photon_control.h"
#endif
#ifndef __MODEL_H__
#include "model/model.h"
#endif

#define MULTI_THREAD
#define NUM_OF_THREADS 4

const int DEF_ABR = 0;
const int DEF_REF = 1;
const int DEF_TRAN = 2;
const float PI = 3.141592654f;
class map_control
{
private:
	// photon related
	Photon_Control absorb_photons;
	Photon_Control p_ctrl;
	Vector3 sun_dir;
	int sun_strength;
	int grid_length;
	int grid_width;

	// model related
	std::vector<Model *> models;
	Vector3 get_Intersact(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &line_point, const Vector3 &line_dir);
	bool IsInTriangle(Vector3 a, Vector3 b, Vector3 c, Vector3 p);

	Vector3 GetNormal(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);
	Vector3 GetPixelColor(const Vector3 &ray_pos, const Vector3 &ray_dir);
	Vector3 GetRayDir(int x, int y, int scene_length, int scene_width, const Vector3 &camera_pos, const Vector3 &camera_ctr, const Vector3 &camera_up);

	// render related
	GLuint vb_id_2d; // texture vb_id for 2D rendering

public:
	// photon related
	map_control(int length, int width, const Vector3 &sun_dir, int sun_strength);
	~map_control()
	{
		for (auto &model : models)
		{
			delete model;
		}
		models.clear();
	}
	void modify_sun_dir(Vector3 sun_dir) { this->sun_dir = sun_dir; }
	void modify_sun_str(int sun_strength) { this->sun_strength = sun_strength; }
	void photons_emit();
	int RussianRoulette(float abr, float ref, float tran);
	void PhotonsModify();
	void printResult()
	{
		for (auto &model : models)
		{
			std::cout << "absorb " << model->getPhotons() << " photons." << std::endl;
		}/*
		for (int i = 0; i < absorb_photons.size();i++) {
			std::cout << absorb_photons[i].pos.x << "," << absorb_photons[i].pos.y << "," << absorb_photons[i].pos.z << std::endl;
		}*/
	}

	// model related
	void add_model(Model *item);
	void add_model(const char *filename, Vector3 pos = Vector3(0.0, 0.0, 0.0));
	void del_model(int index);
	size_t size() { return models.size(); }
	Model *operator[](int index) { return models[index]; }
	void clear_models() { models.clear(); }

	// render related
	void writeBuffer2D(GLdouble *camera, int scrn_width, int scrn_height);
	void deleteBuffer2D();
	void render2D();
	void writeBuffer3D();
	void deleteBuffer3D();
	void render3D(GLdouble *camera);
	Vector3 GetRayColor(int x, int y, int scene_length, int scene_width, const Vector3 &camera_pos, const Vector3 &camera_ctr, const Vector3 &camera_up);
};
#endif
