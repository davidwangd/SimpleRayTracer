#include "Object.h"
#include "RayTracer.h"
#include <cmath>
#include <cstdio>
inline float max(float a, float b){
	return a > b ? a : b;
}

glm::vec3 Object::getColor(const Ray &pray, const glm::vec4& point, int depth){
	glm::vec3 ret = material.ambient+material.emmission;
	glm::vec4 normal = glm::normalize(getNormal(point));
	for (vector<Light>::iterator it = tracer->lights.begin(); it != tracer->lights.end();it++){
		Light light = *it;
		Ray ray;
		ray.pos = point;
		ray.direction = glm::normalize( light.isSpotted ? light.v - point : -light.v);
		ray.pos = point + 1e-3f * ray.direction;
		int v = 1;
		float dis = 1e20f;
		float len = glm::length(light.v - point);
		for (Object* object : tracer -> objects){
			float cur = object -> hitPoint(ray);
			if (cur > eps && cur < dis){
				dis = cur;
				if (!light.isSpotted || dis < len){
					v = 0;
					break;
				}
			}	
		}
		if (v){
			ret = ret + mixcolor( ((max(0, d3dot(normal, ray.direction))) * material.diffuse 
				+ (pow(max(0, d3dot(normal, glm::normalize(ray.direction - pray.direction))), material.shiniss)) * material.specular) 
				, light.color / (light.isSpotted ? (light.c0 + light.c1 * dis + light.c2*dis*dis) : 1));
		}
	}
	if (depth){
		Ray ray;
		ray.pos = point;
		ray.direction = glm::normalize(- 2.f * d3dot(normal, pray.direction) * normal + pray.direction);
		ray.pos = ray.pos + 0.001f * ray.direction;
		Object* hit = nullptr;
		float dis = 1e20f;
		for (Object* object : tracer -> objects){
			float cur = object -> hitPoint(ray);
			if (cur > eps && cur < dis){
				dis = cur;
				hit = object;
			}	
		}
		if (hit != nullptr){
			glm::vec4 newpoint = ray.pos + dis * ray.direction;
			ret = ret + mixcolor(material.specular, hit ->getColor(ray, newpoint, depth - 1));
		}
	}
	return ret;
}