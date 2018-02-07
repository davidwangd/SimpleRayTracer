#ifndef OBJECT_H
#define OBJECT_H
#include "Material.h"
struct Ray;
class RayTracer;
class Object{
public:
	virtual float hitPoint(const Ray& ray) = 0; // 返回hitpoint的路径长度
	virtual glm::vec4 getNormal(const glm::vec4& hitPoint) = 0; // 得到
	glm::vec3 getColor(const Ray &ray, const glm::vec4& point, int depth); // 通过这种方式来得到光照的颜色
	void setTransform(const glm::mat4 trans){
		isTransformed = 1;
		this -> trans = trans;
		this -> inv = glm::inverse(trans);
	}
	void removeTransform(){
		isTransformed = 0;
	}
	void setTracer(RayTracer *tracer){
		this -> tracer = tracer;
	}
	RayTracer* getTracer(){
		return tracer;
	}
	void setMatertial(const Material &m){
		material = m;
	}
protected:
	int isTransformed;
	glm::mat4 trans;
	glm::mat4 inv;
	RayTracer* tracer; // 记录物体所属于的Tracer
	Material material; // 物体的材质
};

#endif