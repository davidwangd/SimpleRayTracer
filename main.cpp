#include "RayTracer.h"
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;
string s;
RayTracer tracer;
int main(){
	printf("Input File: ");
	cin >> s;
	printf("Tracer build OK!\n");
	tracer.readfile(s.c_str());
	tracer.calculate();
	tracer.dump();
}