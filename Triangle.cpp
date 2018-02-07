#include "Triangle.h"
#include "RayTracer.h"
Triangle::Triangle(const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c){
	this -> a = a;
	this -> b = b;
	this -> c = c;
	this -> n = glm::normalize(d3cross(b-a, c-a));

	printf("Object Normal: [%3.f, %.3f, %.3f. %.3f]\n", n.x, n.y, n.z, n.w);
}

Triangle::Triangle(const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c, const glm::vec4 &n){
	this -> a = a;
	this -> b = b;
	this -> c = c;
	this -> n = n;
}

glm::vec4 Triangle::getNormal(const glm::vec4 &hitPoint){
	return isTransformed ? glm::transpose(inv) * n : n;
}

inline int sign(float f){
	if (f < -eps) return -1;
	if (f > eps) return 1;
	return 0;
}

float Triangle::hitPoint(const Ray &ray){
	if (isTransformed){
		printf("Wrong ");
		isTransformed = 0;
	}
	glm::vec4 pos = isTransformed ? inv * ray.pos : ray.pos;
	glm::vec4 direction = isTransformed ? inv * ray.direction : ray.direction; 
	float l = d3dot(direction, n);
	if (sign(l) == 0) return -1;
	float t = -(d3dot(pos-a, n))/l;
//	t = fabs(2/direction.z);
	glm::vec4 P = pos + t * direction;
//	printf("[%.3f, %.3f, %.3f] -> [%.3f, %.3f, %.3f]\n", pos.x, pos.y, pos.z, direction.x, direction.y, direction.z);
//	printf("t = %.3f, [%.3f, %.3f, %.3f]\n", t, P.x, P.y, P.z);

	glm::vec3 v0 = glm::vec3(c - a);
	glm::vec3 v1 = glm::vec3(b - a);
	glm::vec3 v2 = glm::vec3(P - a);

	float dot00 = glm::dot(v0,v0);
	float dot01 = glm::dot(v0,v1);
	float dot02 = glm::dot(v0,v2);
	float dot11 = glm::dot(v1,v1);
	float dot12 = glm::dot(v1,v2);

	float inverDeno = 1/(dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u <= 0 || u >= 1) return -1;
	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v <= 0 || v >= 1) return -1;
	if (u + v <= 1) return t;
	else return -1;

}