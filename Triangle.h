#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"

class Triangle : public Object{
public:
	virtual float hitPoint(const Ray &ray) override;
	virtual glm::vec4 getNormal(const glm::vec4& hitPoint) override;
	Triangle(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);
	Triangle(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, const glm::vec4& n);
private:
	glm::vec4 a, b, c, n;
};

#endif