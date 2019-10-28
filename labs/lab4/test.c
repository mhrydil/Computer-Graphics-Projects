#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include "matLib.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	vec4 v1 = {1, 2, 3, 4};
	vec4 v2 = {5, 6, 7, 8};
	vec4 v3 = {9, 10, 11, 12};
	vec4 v4 = {13, 14, 15, 16};

	mat4 m1 = {v1, v2, v3, v4};
	mat4 m2 = {{4, 3, 2, 1}, {8, 7, 6, 5}, {12, 11, 10, 9}, {16, 15, 14, 13}};
	printf("M1.x: ");
	printVec(m1.x);
	printf("M1:");
	printMat(m1);
	printf("M2:");
	printMat(m2);
	printf("m1 x m2:");
	printMat(matMult(m1, m2));
	printf("m2 x m1:");
	printMat(matMult(m2, m1));


}