#ifndef RAY_TRACER_H
#define RAY_TRACER_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include <string>
#define DEBUG
#define eps 1e-6
using namespace std;
// ���ǵ���ע���һ�㣬���е���ɫ����vec3����ʾ�����е�λ�ö���vec4����ʾ
class Object;
struct Light{
	glm::vec4 v; // ����ǵ��Դ��ʾλ�ã������
	glm::vec3 color; // �����ɫ
	float c0, c1, c2;  // ���ڵ��Դ���ã� 1/(c0+c1*d+c2*d*d)
	int isSpotted; 
};

struct Ray{
	glm::vec4 pos, direction;
};

class RayTracer{
private:
	glm::vec3 eye, center, up;
	glm::mat4 camera;  // ���������
	int height, width; 
	float fovy, fovx; //������Ұ���
	glm::vec3 **grid;
	int maxDetph;  // ����׷��������
	vector<glm::vec4> points;
	int iteration;
	string save;
public:
	vector<Object*> objects; // ����
	vector<Light> lights; // ����
	void setCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	Ray castRay(int x, int y);
	void addLightSource(const Light&);
	void addObject(Object *);
	void calculate();
	void dump(){ dump(save.c_str()); }
	void dump(const char *filename);
	void setSize(int width, int height);
	void readfile(const char *filename);
	~RayTracer();
};

inline float d3dot(const glm::vec4 &a, const glm::vec4 &b){
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline glm::vec4 d3cross(const glm::vec4 &a, const glm::vec4 &b){
	glm::vec3 a1 = glm::vec3(a);
	glm::vec3 a2 = glm::vec3(b);
	return glm::vec4(glm::normalize(glm::cross(a1, a2)), 0);
}

inline glm::vec3 mixcolor(const glm::vec3&a, const glm::vec3 &b){
	return glm::vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}
#endif