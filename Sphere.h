#ifndef SPHERE_H
#define SPEHER_H
#include "Object.h"
class Sphere : public Object{
public:
	virtual float hitPoint(const Ray &ray) override;
	virtual glm::vec4 getNormal(const glm::vec4& hitPoint) override;
	Sphere(const glm::vec4 &center,float r);
private:
	float r;
	glm::vec4 center;
};
#endif