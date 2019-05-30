#include "map_control.h"
#include <random>
#include <time.h>
map_control::map_control(int length, int width, const Vector3 &sun_dir, int sun_strength)
	: grid_length(length), grid_width(width), sun_dir(sun_dir), sun_strength(sun_strength)
{
}

void map_control::photons_emit()
{
	for (float i = 0.0; i < grid_length; i += 0.05)
	{
		for (float j = 0.0; j < grid_width; j += 0.05)
		{
			p_ctrl.photon_emit(sun_dir, Vector3(i, j, 100), Vector3(1.0f, 1.0f, 1.0f) * sun_strength / 255);
		}
	}
	p_ctrl.construct_kdtree();
}

int map_control::Russian_roulette(float abr, float ref, float tran)
{
	srand(time(0));
	float a = (rand() % 100) / 100.0;
	if (a < abr)
		return DEF_ABR;
	else if (a < abr + ref)
		return DEF_REF;
	else
		return DEF_TRAN;
}

void map_control::photons_modify()
{
	while (p_ctrl.size())
	{
		for (int i = 0; i < p_ctrl.size(); i++)
		{
			Vector3 p(200.0f, 200.0f, 0.0f);
			Face *min = NULL;
			GLuint texture_id;
			Model *min_model;
			Vector3 min_normal;
			for (auto &model : models)
			{
				for (auto &mesh : model->meshes)
				{
					for (auto &face : mesh.faces)
					{
						Vector3 normal = get_Normal(model->vertices[face.vertex1.vi],
													model->vertices[face.vertex2.vi],
													model->vertices[face.vertex3.vi]);
						if (p_ctrl.photon_intersect(i, normal,
													model->vertices[face.vertex1.vi] + model->rel_pos,
													model->vertices[face.vertex2.vi] + model->rel_pos,
													model->vertices[face.vertex3.vi] + model->rel_pos))
						{
							if (distance(p_ctrl[i].pos, p_ctrl.get_intersect()) < distance(p_ctrl[i].pos, p))
							{
								p = p_ctrl.get_intersect();
								min = &face;
								texture_id = mesh.texture_id;
								min_model = model;
								min_normal = normal;
							}
						}
					}
				}
			}
			if (min)
			{
				int res = Russian_roulette(min->material.aborption, min->material.reflection, min->material.transmision);
				switch (res)
				{
				case DEF_ABR:
				{
					Vector2 texcoord = min->getTexcoord(p, min_model->vertices, min_model->texcoords);
					Texture texture_info = min_model->getTextureInfo(texture_id);
					int x = ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) % texture_info.w;
					int y = ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) % texture_info.h;
					Vector3 direct = Vector3(texture_info.texture[3 * (x + y * texture_info.w)] / 255.0,
											 texture_info.texture[3 * (x + y * texture_info.w) + 1] / 255.0,
											 texture_info.texture[3 * (x + y * texture_info.w) + 2] / 255.0);
					absorb_photons.photon_emit(p_ctrl[i].dir, p_ctrl[i].pos, direct);
					p_ctrl.photon_absorb(i--);
					min->photons++;
					break;
				}
				case DEF_REF:
				{
					Vector3 ref = p_ctrl.photon_reflect(i, min_normal);
					p_ctrl[i].pos = p;
					p_ctrl[i].dir = ref;
					break;
				}
				case DEF_TRAN:
				{
					Vector3 ref = p_ctrl.photon_refract(i, min_normal, 1.0);
					p_ctrl[i].pos = p;
					p_ctrl[i].dir = ref;
					break;
				}
				default:
					std::cout << "Error result!" << std::endl;
				}
			}
			else
			{
				p_ctrl.photon_absorb(i--);
			}
		}
	}
	absorb_photons.construct_kdtree();
}

void map_control::add_model(Model *item)
{
	models.push_back(item);
}

void map_control::add_model(const char *filename, Vector3 pos)
{
	models.push_back(new Model(filename, pos));
}

void map_control::del_model(int index)
{
	models.erase(models.begin() + index);
}

void map_control::writeBuffer3D()
{
	for (auto &model : models)
	{
		model->writeBuffer();
	}
}

void map_control::deleteBuffer3D()
{
	for (auto &model : models)
	{
		model->deleteBuffer();
	}
}

void map_control::render()
{
	for (auto &model : models)
	{
		model->render();
	}
	glFlush();
}

Vector3 map_control::get_Normal(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
	real_t a = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
	real_t b = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
	real_t c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	return Vector3(a, b, c);
}

Vector3 map_control::get_Intersact(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &line_point, const Vector3 &line_dir)
{
	Vector3 plane_normal = get_Normal(p1, p2, p3);
	real_t d = dotresult(p1 - line_point, plane_normal) / dotresult(line_dir, plane_normal);
	normalize(line_dir);
	return d * line_dir + line_point;
}

bool map_control::in_Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 p)
{
	Vector3 v0 = c - a;
	Vector3 v1 = b - a;
	Vector3 v2 = p - a;
	real_t dot00 = dotresult(v0, v0);
	real_t dot01 = dotresult(v0, v1);
	real_t dot02 = dotresult(v0, v2);
	real_t dot11 = dotresult(v1, v1);
	real_t dot12 = dotresult(v1, v2);
	real_t inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
	real_t u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly
	{
		return false;
	}
	real_t v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly
	{
		return false;
	}
	return u + v <= 1;
}

Vector3 map_control::get_ray_dir(int x, int y, int scene_length, int scene_width, const Vector3 &camera_pos, const Vector3 &camera_ctr, const Vector3 &camera_up)
{
	Vector3 dir = camera_ctr - camera_pos;
	real_t distx = length(dir) * tan(PI / 4) / scene_length * 2;
	real_t disty = length(dir) * tan(PI / 4) / scene_width * 2;
	Vector3 cR = normalize(cross(dir, camera_up));
	Vector3 cU = normalize(camera_up);
	cR *= distx;
	cU *= disty;
	return dir + ((y - scene_width / 2) * cU + (x - scene_length / 2) * cR);
}

Vector3 map_control::get_pixel_color(const Vector3 &ray_pos, const Vector3 &ray_dir)
{
	Vector3 direct, global;
	Vector3 p(1000.0f, 1000.0f, 0.0f);
	Face *min = NULL;
	GLuint texture_id;
	Model *min_model;
	Vector3 min_normal;
	for (auto &model : models)
	{
		for (auto &mesh : model->meshes)
		{
			for (auto &face : mesh.faces)
			{
				Vector3 normal = get_Normal(model->vertices[face.vertex1.vi],
											model->vertices[face.vertex2.vi],
											model->vertices[face.vertex3.vi]);
				Vector3 ins = get_Intersact(model->vertices[face.vertex1.vi] + model->rel_pos,
											model->vertices[face.vertex2.vi] + model->rel_pos,
											model->vertices[face.vertex3.vi] + model->rel_pos,
											ray_pos, ray_dir);
				if (in_Triangle(model->vertices[face.vertex1.vi] + model->rel_pos,
								model->vertices[face.vertex2.vi] + model->rel_pos,
								model->vertices[face.vertex3.vi] + model->rel_pos, ins))
				{
					if (distance(ray_pos, ins) < distance(ray_pos, p))
					{
						p = ins;
						min = &face;
						texture_id = mesh.texture_id;
						min_model = model;
						min_normal = normal;
					}
				}
			}
		}
	}
	Vector3 result;
	if (min)
	{
		Vector2 texcoord = min->getTexcoord(p, min_model->vertices, min_model->texcoords);
		Texture texture_info = min_model->getTextureInfo(texture_id);
		int x = ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) % texture_info.w;
		int y = ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) % texture_info.h;
		direct = Vector3(texture_info.texture[3 * (x + y * texture_info.w)] / 255.0,
						 texture_info.texture[3 * (x + y * texture_info.w) + 1] / 255.0,
						 texture_info.texture[3 * (x + y * texture_info.w) + 2] / 255.0);

		global = Vector3(0.0, 0.0, 0.0);
		int size = 0;
		float d = 5.0;
		auto neighbors = absorb_photons.lookup_kdtree(p, min_normal, &d, &size, 50);
		for (int i = 0; i < size; i++)
		{
			Vector3 color = absorb_photons[neighbors[i].i].power;
			real_t dr1 = dotresult(min_normal, absorb_photons[neighbors[i].i].pos - p);
			real_t dr2 = dotresult(min_normal, ray_pos - p);
			global += color * (dr1 * dr2 >= 0.0f);
		}
		global.x = std::max(std::min(1.0, (double)global.x), 0.0);
		global.y = std::max(std::min(1.0, (double)global.y), 0.0);
		global.z = std::max(std::min(1.0, (double)global.z), 0.0);
		result = dot(direct, global);
		//result = direct;
	}
	else
	{
		result = Vector3(0.0f, 0.0f, 0.0f);
	}
	return result;
}

Vector3 map_control::get_ray_color(int x, int y, int scene_length, int scene_width, const Vector3 &camera_pos, const Vector3 &camera_ctr, const Vector3 &camera_up)
{
	Vector3 dir = get_ray_dir(x, y, scene_length, scene_width, camera_pos, camera_ctr, camera_up);
	Vector3 color = get_pixel_color(camera_pos, dir);
	return color;
}