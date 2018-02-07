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
// 我们得以注意的一点，所有的颜色都用vec3来表示，所有的位置都用vec4来表示
class Object;
struct Light{
	glm::vec4 v; // 如果是点光源表示位置，如果是
	glm::vec3 color; // 光的颜色
	float c0, c1, c2;  // 对于点光源有用， 1/(c0+c1*d+c2*d*d)
	int isSpotted; 
};

struct Ray{
	glm::vec4 pos, direction;
};

class RayTracer{
private:
	glm::vec3 eye, center, up;
	glm::mat4 camera;  // 照相机矩阵
	int height, width; 
	float fovy, fovx; //竖向视野宽度
	glm::vec3 **grid;
	int maxDetph;  // 光线追踪最大深度
	vector<glm::vec4> points;
	int iteration;
	string save;
public:
	vector<Object*> objects; // 物体
	vector<Light> lights; // 光照
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