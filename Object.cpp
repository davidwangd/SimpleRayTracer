#include "Object.h"
#include "RayTracer.h"
#include <cmath>

inline glm::vec3 power(const glm::vec3 &a, float s){
	return glm::vec3(pow(a.x, s), pow(a.y, s), pow(a.z, s));
}

glm::vec3 Object::getColor(const Ray &pray, const glm::vec4& point, int depth){
	glm::vec3 ret = material.ambient+material.emmission;
	glm::vec4 normal = glm::normalize(getNormal(point));
	for (vector<Light>::iterator it = tracer->lights.begin(); it != tracer->lights.end();it++){
		Light light = *it;
		Ray ray;
		ray.pos = point;
		ray.direction = glm::normalize( light.isSpotted ? light.v - point : -light.v);
		Object* hit = 0;
		int v = 1;
		float dis = 1e20;
		for (Object* object : tracer -> objects){
			float cur = object -> hitPoint(ray);
			if (cur > eps && cur < dis){
				hit = object;
				dis = cur;
				if (light.isSpotted || dis < glm::length(light.v - point)){
					v = 0;
					break;
				}
			}	
		}
		if (v){
			ret = ret + ((d3dot(normal, ray.direction)) * material.diffuse 
				+ (d3dot(normal, glm::normalize(ray.direction - pray.direction)) * power(material.specular, material.shiniss)))
				* light.color / (light.c0 + light.c1 * dis + light.c2*dis*dis);
		}
	}
	return ret;
}