#include <stdio.h>
#include <math.h>
#include "matLib.h"

int main(void){
	vec4 v1 = {1, 2, 3, 4};
	vec4 v2 = {5, 6, 7 ,8};
	mat4 m1 = {{1, -5, 9, 13}, {2, 6, -10, 14}, {3, 7, 11, 15}, {4, 8, 12, -16}};
	mat4 m2 = {{4, 8, 12, 16}, {3, 7, 11, 15}, {2, 6, 10, 14}, {1, 5, 9, 13}};
	float s = 3.0;


	printf("s x v1:\n");
	printVec(vecScalar(s, v1));
	printf("\nv1+v2:\n");
	printVec(vecAdd(v1, v2));
	printf("\nv1-v2:\n");
	printVec(vecSub(v1, v2));
	printf("\nv1 (dot) v2:\n");
	printf("%f\n",vecDot(v1, v2));
	printf("\nv1 x v2:\n");
	printVec(vecCross(v1, v2));

	printf("\ns x m1:");
	printMat(matScalar(s, m1));
	printf("\nm1+m2:");
	printMat(matAdd(m1, m2));
	printf("\nm1-m2:");
	printMat(matSub(m1, m2));
	printf("\nm1xm2:");
	printMat(matMult(m1, m2));
	printf("\nm1 Inverse:");
	printMat(matInv(m1));
	printf("\nm1 Transposed:");
	printMat(matTranspose(m1));
	printf("\nm1 x v1:\n");
	printVec(matVec(m1, v1));

	printf("Determinant of m1: %f\n", determ4(m1, matMinor(m1)));
	printf("Inverse of m1:");
	printMat(matInv(m1));
	printf("Determinant of m2: %f", determ4(m2, matMinor(m2)));

	printf("\nInverse of m2: ");
	printMat(matInv(m2));

	printf("\n(v1 x v2).(v1) = ");
	vec4 v1crossv2 = vecCross(v1, v1);
	printVec(v1crossv2);
	//printf("%f\n", vecDot(v1crossv2, v1));

	vec4 v1Addv2 = vecAdd(v1, v2);
	float v1Addv2dotv1 = vecDot(v1Addv2, v1);


	float test2 = vecDot(v1, v1) + vecDot(v1, v2);
	printf("%f\n", v1Addv2dotv1);
	printf("%f\n", test2);

	mat4 m3 = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	mat4 m3Trans = matTranspose(m3);
	mat4 m3TransInv = matInv(m3Trans);
	printMat(m3TransInv);

	mat4 m4 = matMult(m1, m2);
	printMat(m4);
	mat4 m4Times2 = matScalar(2, m4);
	printMat(m4Times2);
	mat4 m4Divided2 = matScalar(.5, m4Times2);
	printMat(m4Divided2);


	vec4 test3 = vecSub(v1, vecAdd(v1, v2));
	printf("v1-(v1+v2):\n");
	printVec(test3);

	vec4 test4 = {1, 2, 3, 4};
	printf("Magnitude of test4: %f\n", vecMag(test4));

	printf("Normal vector of test4:\n");
	printVec(vecNorm(test4));





	//printMat(matMult(m1, matInv(m1))); //m1 * inverse(m1) = 1

	//printMat(matMult(m1, matInv(m1)));
}