#include "map_control.h"
#include <random>
#include <time.h>
#include <thread>
inline Vector3 GetReflect(const Vector3& dir, const Vector3& norm) {
	Vector3 normal = normalize(norm);
	return (dir - 2 * dot((dot(dir, normal)), normal));
}

inline Vector3 GetRefract(const Vector3& dir, const Vector3 & norm, real_t coef) {
	return Vector3(-sqrt(1 - coef * coef * (1 - squared_length(dot(dir, norm)) * squared_length(dot(dir, norm)))) * norm + coef * (dir + squared_length(dot(dir, norm)) * norm));
}

inline Vector3 GetNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
	real_t a = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
	real_t b = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
	real_t c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	return Vector3(a, b, c);
}

inline Vector3 get_Intersact(const Vector3& plane_normal, const Vector3& plane_point, const Vector3& line_point, const Vector3& line_dir) {
	real_t d = dotresult(plane_point - line_point, plane_normal) / dotresult(line_dir, plane_normal);
	normalize(line_dir);
	return d * line_dir + line_point;
}

inline bool IsInTriangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& p)
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


map_control::map_control(int length, int width, const Vector3 &sun_dir, int sun_strength)
	: grid_length(length), grid_width(width), sun_dir(sun_dir), sun_strength(sun_strength)
{
	srand(time(0));
}

void map_control::photons_emit()
{
	for (real_t i = 0.0f; i < grid_length; i += 0.05f)
	{
		for (real_t j = 0.0f; j < grid_width; j += 0.05f)
		{
			p_ctrl.PhotonEmit(sun_dir, Vector3(i, j, 100.0f), Vector3(1.0f, 1.0f, 1.0f) * (real_t)sun_strength / 255);
		}
	}
	p_ctrl.ConstructKDTree();
}

int map_control::RussianRoulette(float abr, float ref, float tran)
{
	float a = (rand() % 100) / 100.0f;
	if (a < abr)
		return DEF_ABR;
	else if (a < abr + ref)
		return DEF_REF;
	else
		return DEF_TRAN;
}

void map_control::PhotonsModify()
{
	while (p_ctrl.size())
	{
		for (int i = 0; i < p_ctrl.size(); i++)
		{
			Vector3 p(200.0f, 200.0f, 0.0f);
			Face* min = NULL;
			GLuint texture_id;
			Model* min_model;
			Vector3 min_normal;
			for (auto& model : models)
			{
				for (auto& mesh : model->meshes)
				{
					for (auto& face : mesh.faces)
					{
						Vector3 a = model->vertices[face.vertex1.vi] + model->rel_pos;
						Vector3 b = model->vertices[face.vertex2.vi] + model->rel_pos;
						Vector3 c = model->vertices[face.vertex3.vi] + model->rel_pos;
						Vector3 normal = GetNormal(a, b, c);
						Vector3 intersect = get_Intersact(a, b, c, p_ctrl[i].pos, p_ctrl[i].dir);
						if(IsInTriangle(a, b, c, intersect))
						{
							if (distance(p_ctrl[i].pos, intersect) < distance(p_ctrl[i].pos, p))
							{
								p = intersect;
								min = &face;
								texture_id = mesh.texture_id;
								min_model = model;
								if (dotresult(normal, p_ctrl[i].dir) > 0.0f) {
									normal *= -1.0;
								}
								min_normal = normal;
							}
						}
					}
				}
			}
			if (min)
			{
				int res = RussianRoulette(min->material.aborption, min->material.reflection, min->material.transmision);
				switch (res)
				{
				case DEF_ABR:
				{
					absorb_photons.PhotonEmit(min_normal, p, p_ctrl[i].power);
					p_ctrl.PhotonAbsorb(i--);
					min->photons++;
					break;
				}
				case DEF_REF:
				{
					Vector3 ref = GetReflect(p_ctrl[i].dir, min_normal);
					p_ctrl[i].pos = p;
					p_ctrl[i].dir = ref;
					break;
				}
				case DEF_TRAN:
				{
					Vector3 ref = GetRefract(p_ctrl[i].dir, min_normal, 1.0);
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
				p_ctrl.PhotonAbsorb(i--);
			}
		}
	}
	absorb_photons.ConstructKDTree();
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

void map_control::render3D(GLdouble *camera)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2],
			  camera[3], camera[4], camera[5],
			  camera[6], camera[7], camera[8]);
	for (auto &model : models)
	{
		model->render();
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glFlush();
}

void map_control::writeBuffer2D(GLdouble *camera, int scrn_width, int scrn_height)
{
	std::vector<float> buffer;

	std::cout << "Camera position: " << std::endl;
	std::cout << camera[0] << " " << camera[1] << " " << camera[2] << std::endl;
	std::cout << camera[3] << " " << camera[4] << " " << camera[5] << std::endl;
	std::cout << camera[6] << " " << camera[7] << " " << camera[8] << std::endl;

#ifdef MULTI_THREAD
	std::thread t[NUM_OF_THREADS];
	std::vector<float> tbuffer[NUM_OF_THREADS];
#endif
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
#ifdef MULTI_THREAD
		t[i] = std::thread([=, &tbuffer]() {
			std::cout << "Thread " << i << ": rendering row "
					  << i * (scrn_height / NUM_OF_THREADS) << " to "
					  << (i + 1) * (scrn_height / NUM_OF_THREADS) - 1 << std::endl;
#endif
			Vector3 rgb(1.0, 1.0, 1.0);
			for (int y = i * (scrn_height / NUM_OF_THREADS); y < (i + 1) * (scrn_height / NUM_OF_THREADS); y++)
			{
				for (int x = 0; x < scrn_width; x++)
				{
					rgb = GetRayColor(x, y, scrn_width, scrn_height,
										Vector3((real_t)camera[0], (real_t)camera[1], (real_t)camera[2]),
										Vector3((real_t)camera[3], (real_t)camera[4], (real_t)camera[5]),
										Vector3((real_t)camera[6], (real_t)camera[7], (real_t)camera[8]));
#ifdef MULTI_THREAD
					tbuffer[i].push_back(rgb.x);
					tbuffer[i].push_back(rgb.y);
					tbuffer[i].push_back(rgb.z);
#else
				buffer.push_back(rgb.x);
				buffer.push_back(rgb.y);
				buffer.push_back(rgb.z);
#endif
				}
				if (!(y % 100))
				{
					std::cout << "Row " << y << " rendered." << std::endl;
				}
			}
#ifdef MULTI_THREAD
		});
#endif
	}

#ifdef MULTI_THREAD
	for (int i = 0; i < NUM_OF_THREADS; ++i)
	{
		t[i].join();
	}
	for (int i = 0; i < NUM_OF_THREADS; ++i)
		buffer.insert(buffer.end(), tbuffer[i].begin(), tbuffer[i].end());
#endif

	std::cout << "Writing to buffer.." << std::endl;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &vb_id_2d);
	glBindTexture(GL_TEXTURE_2D, vb_id_2d);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
				 (GLsizei)scrn_width, (GLsizei)scrn_height,
				 0, GL_RGB, GL_FLOAT, &buffer.at(0));
	glDisable(GL_TEXTURE_2D);
}

void map_control::render2D()
{
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, vb_id_2d);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 0);
	glVertex3f(1, -1, -1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1, -1);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glFlush();
};

Vector3 map_control::GetNormal(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
	real_t a = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
	real_t b = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
	real_t c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	return Vector3(a, b, c);
}

Vector3 map_control::get_Intersact(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &line_point, const Vector3 &line_dir)
{
	Vector3 plane_normal = GetNormal(p1, p2, p3);
	real_t d = dotresult(p1 - line_point, plane_normal) / dotresult(line_dir, plane_normal);
	normalize(line_dir);
	return d * line_dir + line_point;
}

bool map_control::IsInTriangle(Vector3 a, Vector3 b, Vector3 c, Vector3 p)
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

Vector3 map_control::GetRayDir(int x, int y, int scene_length, int scene_width, const Vector3 &camera_pos, const Vector3 &camera_ctr, const Vector3 &camera_up)
{
	Vector3 dir = camera_ctr - camera_pos;
	Vector3 cR = cross(dir, camera_up);
	Vector3 cU = cross(cR, dir);
	real_t AR = length(cU) / length(cR);
	real_t dist = tan(PI / 4) * 2 / length(dir);
	Vector3 t = dir + dist * (((real_t)y - (real_t)scene_width / 2) / scene_width * cU + AR * ((real_t)x - (real_t)scene_length / 2) / scene_length * cR);
	t = normalize(t);
	return t;
}

Vector3 map_control::GetPixelColor(const Vector3 &ray_pos, const Vector3 &ray_dir)
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
				Vector3 a = model->vertices[face.vertex1.vi] + model->rel_pos;
				Vector3 b = model->vertices[face.vertex2.vi] + model->rel_pos;
				Vector3 c = model->vertices[face.vertex3.vi] + model->rel_pos;
				Vector3 normal = GetNormal(a, b, c);
				Vector3 intersect = get_Intersact(a, b, c, ray_pos, ray_dir);
				if (IsInTriangle(a, b, c, intersect))
				{
					if (distance(ray_pos, intersect) < distance(ray_pos, p))
					{
						p = intersect;
						min = &face;
						texture_id = mesh.texture_id;
						min_model = model;
						if (dotresult(normal, ray_dir) > 0.0f) {
							normal *= -1.0;
						}
						min_normal = normal;
					}
				}
			}
		}
	}
	Vector3 result;
	if (min)
	{
		Vector2 texcoord = min->getTexcoord(p - min_model->rel_pos, min_model->vertices, min_model->texcoords);
		Texture texture_info = min_model->getTextureInfo(texture_id);
		int x = ((int)(texture_info.w * texcoord.x) % texture_info.w + texture_info.w) % texture_info.w;
		int y = ((int)(texture_info.h * texcoord.y) % texture_info.h + texture_info.h) % texture_info.h;
		direct = Vector3(texture_info.texture[3 * (x + y * texture_info.w)] / 255.0f,
						 texture_info.texture[3 * (x + y * texture_info.w) + 1] / 255.0f,
						 texture_info.texture[3 * (x + y * texture_info.w) + 2] / 255.0f);

		global = Vector3(0.0, 0.0, 0.0);
		int size = 0;
		float d = 0.0;
		int count = 0;
		auto neighbors = absorb_photons.LookuptKDTree(p, min_normal, 0.0025f, d, size, 50);
		for (int i = 0; i < size; i++)
		{
			real_t dist = distance(absorb_photons[neighbors[i].i].pos, p);
			Vector3 color = absorb_photons[neighbors[i].i].power;
			if (dist >= 1.0f) {
				color /= dist;
			}
			real_t dr1 = dotresult(absorb_photons[neighbors[i].i].dir, absorb_photons[neighbors[i].i].pos - p);
			global += color;
			count++;
		}
		if (count) {
			global /= (real_t)count;
			result = dot(direct, global);
		}
		else {
			result = Vector3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		result = Vector3(0.0f, 0.0f, 0.0f);
	}
	return result;
}

Vector3 map_control::GetRayColor(int x, int y, int scene_length, int scene_width, const Vector3 &camera_pos, const Vector3 &camera_ctr, const Vector3 &camera_up)
{
	Vector3 dir = GetRayDir(x, y, scene_length, scene_width, camera_pos, camera_ctr, camera_up);
	Vector3 color = GetPixelColor(camera_pos, dir);
	return color;
}