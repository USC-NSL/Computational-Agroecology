#ifndef __MATERIAL_H__
#define __MATERIAL_H__
class Material
{
public:
	Material(float aborption = 0.5, float reflection = 0.5, float transmision = 0.0) :aborption(aborption), reflection(reflection), transmision(transmision) {}
	~Material() {};
	float aborption;
	float reflection;
	float transmision;
};
#endif /* __MATERIAL_H__ */