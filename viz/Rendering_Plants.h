#ifndef _RENDERING_PLANTS_H_
#define _RENDERING_PLANTS_H_

#include "stdafx.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


typedef struct {
	GLuint vb_id;  // vertex buffer id
	int numTriangles;
	size_t material_id;
} DrawObject;


class Plants {
private:
	std::string Plant_Type;
	int Plant_Pos_X, Plant_Pos_Y;
	int Plant_Maturity;
	std::vector<DrawObject> gDrawObjects;
	std::map<std::string, GLuint> textures;
protected:
	std::vector<tinyobj::material_t> materials;
public:
	Plants(std::string Plant_Type, int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	~Plants();
	void LoadObjModel(const char* filename);
	void Render();
	int getX() const { return Plant_Pos_X; }
	int getY() const { return Plant_Pos_Y; }
	void modifyPos(int x, int y);
	int Maturity() const { return Plant_Maturity; }
	std::string Type() const { return Plant_Type; }
};

class Corn : public Plants {
private:
public:
	Corn(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	void Rendering();
	~Corn();
};

class Squash : public Plants {
private:
public:
	Squash(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	void Rendering();
	~Squash();
};

class Bean : public Plants {
private:
public:
	Bean(int Plant_Pos_X, int Plant_Pos_Y, int Plant_Maturity);
	void Rendering();
	~Bean();
};
#endif // !_RENDERING_PLANTS_H_


