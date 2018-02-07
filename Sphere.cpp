#include "Sphere.h"
#include "RayTracer.h"
#include <cmath>

float Sphere::hitPoint(const Ray &ray){
	glm::vec4 direction = isTransformed ? inv * ray.direction : ray.direction;
	glm::vec4 pos = isTransformed ? inv * ray.pos : ray.pos;
	
	float a = d3dot(direction, direction);
	float b = 2 * d3dot(pos - center, direction);
	float c = d3dot(pos-center, pos-center) - r*r;
	if (b * b - 4 * a * c <= 0) return -1;
	float delta = sqrt(b*b-4*a*c);
	float x1 = (-b-delta)/2/a, x2 = (-b+delta)/2/a;
	if (x1 > eps) return x1;
	if (x2 > eps) return x2;
	return -1;
}

glm::vec4 Sphere::getNormal(const glm::vec4& hitPoint){
	return isTransformed ? glm::transpose(inv) * (inv * hitPoint - inv * center) : hitPoint - center;
}

Sphere::Sphere(const glm::vec4 &center, float r){
	this->center = center;
	this->r = r;
}