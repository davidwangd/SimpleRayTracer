#ifndef OBJECT_H
#define OBJECT_H
#include "Material.h"
struct Ray;
class RayTracer;
class Object{
public:
	virtual float hitPoint(const Ray& ray) = 0; // ����hitpoint��·������
	virtual glm::vec4 getNormal(const glm::vec4& hitPoint) = 0; // �õ�
	glm::vec3 getColor(const Ray &ray, const glm::vec4& point, int depth); // ͨ�����ַ�ʽ���õ����յ���ɫ
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
	RayTracer* tracer; // ��¼���������ڵ�Tracer
	Material material; // ����Ĳ���
};

#endif