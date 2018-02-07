#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
struct Material{
	glm::vec3 emmission; // ×Ô·¢¹â
	glm::vec3 diffuse, specular;
	glm::vec3 ambient;
	float shiniss;
};

#endif
