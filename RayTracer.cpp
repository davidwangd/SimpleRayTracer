#include "RayTracer.h"
#include "Object.h"
#include "Sphere.h"
#include "Material.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
void RayTracer::setSize(int width, int height){
	this->width = width;
	this->height = height;
	grid = new glm::vec3*[width];
	for (int i = 0;i < width;i++){
		grid[i] = new glm::vec3[height];
	}
}

void RayTracer::setCamera(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3& up){
	this -> eye = eye;
	this -> center = center;
	this -> up = up;
	this -> camera = glm::lookAt(eye, center, up) ;
	// 我们考虑向z轴正半轴射出的RayCast,
	// 通过与Camer矩阵想成得到原始坐标系中的光纤
}

Ray RayTracer::castRay(int x, int y){
	Ray ret;
	ret.pos = glm::vec4(0,0,0,1);
	ret.direction = glm::normalize(glm::vec4(glm::tan(fovx/2)*(x-(width/2)+0.5)/(width/2+0.5), glm::tan(fovy/2)*((height/2)-y+0.5)/(height/2+0.5),-1,0));
	return ret;
}

void RayTracer::addObject(Object *object){
	objects.push_back(object);
	object->setTracer(this);
}

void RayTracer::addLightSource(const Light &light){
	lights.push_back(light);
}

void RayTracer::calculate(){
	printf("width = %d, height = %d, fovx = %.3f, fovy = %.3f\n", width, height, fovx, fovy);
	printf("Object Count = %d, Light Count = %d\n maxDepth = %d", objects.size(), lights.size(), maxDetph);
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			Ray ray = castRay(x, y);
#ifdef DEBUG
	if ((x == 0 || x == width/2 || x == width - 1) && (y == 0 || y == height/2 || y == height - 1))
			printf("[%.3f, %.3f, %.3f] -> [%.3f, %.3f, %.3f]\n", ray.pos.x, ray.pos.y, ray.pos.z, ray.direction.x, ray.direction.y, ray.direction.z);
#endif
			Object* hit = 0;
			float dis = 1e20;
			for (Object* object : objects){
				float cur = object -> hitPoint(ray);
				if (cur > 0 && cur < dis){
					hit = object;
					dis = cur;
				}
			}
			if (dis > 1e10){
				grid[x][y] = glm::vec3(0,0,0);
			}else{
				glm::vec4 point = ray.pos + dis * ray.direction;
			//	printf("Hit: dis = %.3f [%.3f, %.3f, %.3f %.3f]\n",dis, point.x, point.y, point.z, point.w);
				grid[x][y] = hit -> getColor(ray, point, maxDetph);
			}
		}
		printf("&");
	}
}

void RayTracer::dump(const char *filename){

	FILE *file;
	fopen_s(&file, filename, "w");
	fprintf(file, "%d %d\n", width, height);
	for (int i = 0;i < width;i++)
		for (int j = 0;j < height;j++){
			for (int k = 0; k < 3;k++)
				fprintf(file, "%f ", grid[i][j][k]);
			fprintf(file, "\n");
		}
	fclose(file);
	system("python show.py");
	system("pause");
}

RayTracer::~RayTracer(){
	for (Object *object : objects) delete object;
	for (int i = 0;i < width;i++) delete[] grid[i];
	delete[] grid;
}

bool readvals(stringstream &s, const int numvals, float* values) 
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i]; 
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n"; 
			return false;
		}
	}
	return true; 
}

void RayTracer::readfile(const char *filename){ 
	printf("Start Read File");
	ifstream in;
	in.open(filename);
	if (!in.is_open()){
		printf("Fatel Error, File haven't been opened\n");
		return;
	}else
		printf("Successfully Open file %s\n", filename);
	stack<glm::mat4> matStack;
	// 首先由单位矩阵
	matStack.push(glm::mat4(1.0));
	string str, cmd;
	Material material;
	Light light;
	light.c0 = 1;
	light.c1 = 0;
	light.c2 = 0;
	save = string("output.txt");
	material.ambient = glm::vec3(0.2, 0.2, 0.2);
	while (getline(in, str)){
		stringstream s(str);
		if (!(s >> cmd)) continue;
		float numbers[20];
		if (cmd == "#"){
			continue;
		}if (cmd == "size"){
			readvals(s, 2, numbers);
			this -> setSize((int)(numbers[0]+0.5), (int)(numbers[1]+0.5));
		}else if (cmd == "camera"){
			readvals(s, 10, numbers);
			this -> setCamera(
				glm::vec3(numbers[0], numbers[1], numbers[2]),
				glm::vec3(numbers[3], numbers[4], numbers[5]),
				glm::vec3(numbers[6], numbers[7], numbers[8])
			);
			fovy = glm::radians(numbers[9]);
			fovx = glm::atan(glm::tan(fovy/2) * width / height) * 2;
		}else if (cmd == "maxdepth"){
			readvals(s, 1, numbers);
			maxDetph = (int)(0.5+numbers[0]);
		}else if (cmd == "output"){
		//	s >> save;
		}else if (cmd == "sphere"){
			readvals(s, 4, numbers);
			glm::vec4 pos = glm::vec4(numbers[0], numbers[1], numbers[2], 1);
			Object *sphere = new Sphere(pos, numbers[3]);
			sphere -> setTransform(camera * matStack.top());
			sphere -> setMatertial(material);
			this -> addObject(sphere);
		}else if (cmd == "tri"){
			readvals(s, 3, numbers);
			int a = (int)(numbers[0] + 0.5);
			int b = (int)(numbers[1] + 0.5);
			int c = (int)(numbers[2] + 0.5);
			glm::mat4 m = camera * matStack.top();
			Object *tri = new Triangle(m * points[a], m * points[b], m * points[c]);
			for (int i = 0;i < 4;i++){
				for (int j = 0;j < 4;j++)
					printf("%.3f ", m[i][j]);
				printf("\n");
			}
			glm::vec4 A = m * points[a];
			glm::vec4 B = m * points[b];
			glm::vec4 C = m * points[c];
			printf("X: %.3f %.3f %.3f\n", A.x, A.y, A.z);
			printf("Y: %.3f %.3f %.3f\n", B.x, B.y, B.z);
			printf("Z: %.3f %.3f %.3f\n", C.x, C.y, C.z);
			printf("Color1 : %.3f %.3f %.3f\n", material.ambient.x, material.ambient.y, material.ambient.z);
			printf("Color2 : %.3f %.3f %.3f\n\n", material.emmission.x, material.emmission.y, material.emmission.z);
			tri -> setMatertial(material);
			tri -> removeTransform();
			this -> addObject(tri);
		}else if (cmd == "vertex"){
			readvals(s, 3, numbers);
			points.push_back(glm::vec4(numbers[0], numbers[1], numbers[2], 1));
		}else if (cmd == "translate"){
			readvals(s, 3, numbers);
			matStack.top() = glm::translate(matStack.top(), glm::vec3(numbers[0], numbers[1], numbers[2]));
		}else if (cmd == "scale"){
			readvals(s, 3, numbers);
			matStack.top() = glm::scale(matStack.top(), glm::vec3(numbers[0], numbers[1], numbers[2]));
		}else if (cmd == "rotate"){
			readvals(s, 4, numbers);
			matStack.top() = glm::rotate(matStack.top(), numbers[3], glm::vec3(numbers[0], numbers[1], numbers[2]));
		}else if (cmd == "pushTransform"){
			matStack.push(matStack.top());
		}else if (cmd == "popTransform"){
			matStack.pop();
		}else if (cmd == "directional"){
			readvals(s, 6, numbers);
			light.v = glm::vec4(numbers[0], numbers[1], numbers[2], 0);
			light.color = glm::vec3(numbers[3], numbers[4], numbers[5]);
			light.isSpotted = 0;
			this -> addLightSource(light);
		}else if (cmd == "point"){
			readvals(s, 6, numbers);
			light.v = glm::vec4(numbers[0], numbers[1], numbers[2], 0);
			light.color = glm::vec3(numbers[3], numbers[4], numbers[5]);
			light.isSpotted = 1;
			this -> addLightSource(light);
		}else if (cmd == "attenuation"){
			readvals(s, 3, numbers);
			light.c0 = numbers[0];
			light.c1 = numbers[1];
			light.c2 = numbers[2];
		}else if (cmd == "ambient"){
			readvals(s, 3, numbers);
			material.ambient = glm::vec3(numbers[0], numbers[1], numbers[2]);
		}else if (cmd == "diffuse"){
			readvals(s, 3, numbers);
			material.diffuse = glm::vec3(numbers[0], numbers[1], numbers[2]);
		}else if (cmd == "specular"){
			readvals(s, 3, numbers);
			material.specular = glm::vec3(numbers[0], numbers[1], numbers[2]);
		}else if (cmd == "emission"){
			readvals(s, 3, numbers);
			material.emmission = glm::vec3(numbers[0], numbers[1], numbers[2]);
		}else if (cmd == "shininess"){
			readvals(s, 1, &material.shiniss);
		}
	}
}