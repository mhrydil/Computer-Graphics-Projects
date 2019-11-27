#define GL_SILENCE_DEPRECATION


#include <stdio.h>
#include <math.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include "matLib.h"


//print the vector in form [2.01 1.02 3.39 4.22]
void printVec(vec4 v){
	printf("[%.5f %.5f %.5f %.5f]\n", v.x, v.y, v.z, v.w);
}

// scalar multiply vector by x
vec4 vecScalar(GLfloat s, vec4 vec){
	vec4 v;
	v.x = s*vec.x;
	v.y = s*vec.y;
	v.z = s*vec.z;
	v.w = s*vec.w;
	return v;
}

//add v1 to v2
vec4 vecAdd(vec4 v1, vec4 v2){
	vec4 v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	v.w = v1.w + v2.w;
	return v;
}

// subtract v1 from v2
vec4 vecSub(vec4 v1, vec4 v2){
	vec4 v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	v.w = v1.w - v2.w;
	return v;
}

//dot product of 2 vectors
GLfloat vecDot(vec4 v1, vec4 v2){
	GLfloat result = 0;
	result = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) + (v1.w*v2.w);
	return result;
}


//cross product of 2 vectors(ignoring the 4th element in each vector)
vec4 vecCross(vec4 v1, vec4 v2){
	vec4 result;
	result.x = (v1.y*v2.z)-(v2.y*v1.z);
	result.y = (v2.x*v1.z)-(v1.x*v2.z);
	result.z = (v1.x*v2.y)-(v2.x*v1.y);
	result.w = 0;
	return result;
}

float vecMag(vec4 v1){
	return sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z + v1.w*v1.w);
}

vec4 vecNorm(vec4 vec){ //takes a vec4(vec), returns a normalized vector of vec
	vec4 result = vecScalar(1/vecMag(vec), vec);
	return result;

}

vec4 product(vec4 u, vec4 v)
{
    vec4 result;
    result.x = u.x * v.x;
    result.y = u.y * v.y;
    result.z = u.z * v.z;
    result.w = u.w * v.w;
    return result;
}

//Beginning of Matrix Operations


mat4 matTranspose(mat4 m){
	vec4 x = {m.x.x, m.y.x, m.z.x, m.w.x};
	vec4 y = {m.x.y, m.y.y, m.z.y, m.w.y};
	vec4 z = {m.x.z, m.y.z, m.z.z, m.w.z};
	vec4 w = {m.x.w, m.y.w, m.z.w, m.w.w};
	mat4 result = {x, y, z, w};
	return result;
}

void printMat(mat4 m){
	printf("\n[%.2f ", m.x.x);
	printf("%.2f ", m.y.x);
	printf("%.2f ", m.z.x);
	printf("%.2f]\n", m.w.x);
	printf("[%.2f ", m.x.y);
	printf("%.2f ", m.y.y);
	printf("%.2f ", m.z.y);
	printf("%.2f]\n", m.w.y);
	printf("[%.2f ", m.x.z);
	printf("%.2f ", m.y.z);
	printf("%.2f ", m.z.z);
	printf("%.2f]\n", m.w.z);
	printf("[%.2f ", m.x.w);
	printf("%.2f ", m.y.w);
	printf("%.2f ", m.z.w);
	printf("%.2f]\n", m.w.w);
}

mat4 matScalar(GLfloat s, mat4 mat){
	mat4 result;
	result.x = vecScalar(s, mat.x);
	result.y = vecScalar(s, mat.y);
	result.z = vecScalar(s, mat.z);
	result.w = vecScalar(s, mat.w);
	return result;
}

mat4 matAdd(mat4 m1, mat4 m2){
	mat4 result;
	result.x = vecAdd(m1.x, m2.x);
	result.y = vecAdd(m1.y, m2.y);
	result.z = vecAdd(m1.z, m2.z);
	result.w = vecAdd(m1.w, m2.w);
	return result;
}

mat4 matSub(mat4 m1, mat4 m2){
	mat4 result;
	result.x = vecSub(m1.x, m2.x);
	result.y = vecSub(m1.y, m2.y);
	result.z = vecSub(m1.z, m2.z);
	result.w = vecSub(m1.w, m2.w);
	return result;
}

// c11 = (a11 ×b11)+(a12 ×b21)+(a13 ×b31)+(a14 ×b41) 
// c21 = (a21 ×b11)+(a22 ×b21)+(a23 ×b31)+(a24 ×b41) 
// c31 = (a31 ×b11)+(a32 ×b21)+(a33 ×b31)+(a34 ×b41) 
// c41 = (a41 ×b11)+(a42 ×b21)+(a43 ×b31)+(a44 ×b41) 
// c12 = (a11 ×b12)+(a12 ×b22)+(a13 ×b32)+(a14 ×b42) 
// c22 = (a21 ×b12)+(a22 ×b22)+(a23 ×b32)+(a24 ×b42) 
// c32 = (a31 ×b12)+(a32 ×b22)+(a33 ×b32)+(a34 ×b42) 
// c42 = (a41 ×b12)+(a42 ×b22)+(a43 ×b32)+(a44 ×b42) 
// c13 = (a11 ×b13)+(a12 ×b23)+(a13 ×b33)+(a14 ×b43) 
// c23 = (a21 ×b13)+(a22 ×b23)+(a23 ×b33)+(a24 ×b43) 
// c33 = (a31 ×b13)+(a32 ×b23)+(a33 ×b33)+(a34 ×b43) 
// c43 = (a41 ×b13)+(a42 ×b23)+(a43 ×b33)+(a44 ×b43) 
// c14 = (a11 ×b14)+(a12 ×b24)+(a13 ×b34)+(a14 ×b44) 
// c24 = (a21 ×b14)+(a22 ×b24)+(a23 ×b34)+(a24 ×b44) 
// c34 = (a31 ×b14)+(a32 ×b24)+(a33 ×b34)+(a34 ×b44) 
// c44 = (a41 ×b14)+(a42 ×b24)+(a43 ×b34)+(a44 ×b44)

mat4 matMult(mat4 m1, mat4 m2){
	mat4 result;
	result.x.x = (m1.x.x * m2.x.x) + (m1.y.x * m2.x.y) + (m1.z.x * m2.x.z) + (m1.w.x * m2.x.w);
	result.x.y = (m1.x.y * m2.x.x) + (m1.y.y * m2.x.y) + (m1.z.y * m2.x.z) + (m1.w.y * m2.x.w);
	result.x.z = (m1.x.z * m2.x.x) + (m1.y.z * m2.x.y) + (m1.z.z * m2.x.z) + (m1.w.z * m2.x.w);
	result.x.w = (m1.x.w * m2.x.x) + (m1.y.w * m2.x.y) + (m1.z.w * m2.x.z) + (m1.w.w * m2.x.w);

	result.y.x = (m1.x.x * m2.y.x) + (m1.y.x * m2.y.y) + (m1.z.x * m2.y.z) + (m1.w.x * m2.y.w);
	result.y.y = (m1.x.y * m2.y.x) + (m1.y.y * m2.y.y) + (m1.z.y * m2.y.z) + (m1.w.y * m2.y.w);
	result.y.z = (m1.x.z * m2.y.x) + (m1.y.z * m2.y.y) + (m1.z.z * m2.y.z) + (m1.w.z * m2.y.w);
	result.y.w = (m1.x.w * m2.y.x) + (m1.y.w * m2.y.y) + (m1.z.w * m2.y.z) + (m1.w.w * m2.y.w);

	result.z.x = (m1.x.x * m2.z.x) + (m1.y.x * m2.z.y) + (m1.z.x * m2.z.z) + (m1.w.x * m2.z.w);
	result.z.y = (m1.x.y * m2.z.x) + (m1.y.y * m2.z.y) + (m1.z.y * m2.z.z) + (m1.w.y * m2.z.w);
	result.z.z = (m1.x.z * m2.z.x) + (m1.y.z * m2.z.y) + (m1.z.z * m2.z.z) + (m1.w.z * m2.z.w);
	result.z.w = (m1.x.w * m2.z.x) + (m1.y.w * m2.z.y) + (m1.z.w * m2.z.z) + (m1.w.w * m2.z.w);

	result.w.x = (m1.x.x * m2.w.x) + (m1.y.x * m2.w.y) + (m1.z.x * m2.w.z) + (m1.w.x * m2.w.w);
	result.w.y = (m1.x.y * m2.w.x) + (m1.y.y * m2.w.y) + (m1.z.y * m2.w.z) + (m1.w.y * m2.w.w);
	result.w.z = (m1.x.z * m2.w.x) + (m1.y.z * m2.w.y) + (m1.z.z * m2.w.z) + (m1.w.z * m2.w.w);
	result.w.w = (m1.x.w * m2.w.x) + (m1.y.w * m2.w.y) + (m1.z.w * m2.w.z) + (m1.w.w * m2.w.w);

	return result;
}

GLfloat determ3(GLfloat x1, GLfloat x2, GLfloat x3, GLfloat y1, GLfloat y2, GLfloat y3, GLfloat z1, GLfloat z2, GLfloat z3){
	return ((x1*y2*z3) + (x2*y3*z1) + (x3*y1*z2) - (z1*y2*x3) - (z2*y3*x1) - (z3*y1*x2));
}

GLfloat determ4(mat4 m, mat4 minor){
	return (m.x.x*minor.x.x - m.x.y*minor.x.y + m.x.z*minor.x.z - m.x.w*minor.x.w);
}


mat4 matMinor(mat4 m){
	mat4 result;
	//printf("%f", determ3(m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w));
	result.x.x = determ3(m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w);
	result.x.y = determ3(m.y.x, m.z.x, m.w.x, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w);
	result.x.z = determ3(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.w, m.z.w, m.w.w);
	result.x.w = determ3(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z);

	result.y.x = determ3(m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w);
	result.y.y = determ3(m.x.x, m.z.x, m.w.x, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w);
	result.y.z = determ3(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.w, m.z.w, m.w.w);
	result.y.w = determ3(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z);

	result.z.x = determ3(m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w);
	result.z.y = determ3(m.x.x, m.y.x, m.w.x, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w);
	result.z.z = determ3(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.w, m.y.w, m.w.w);
	result.z.w = determ3(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z);

	result.w.x = determ3(m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w);
	result.w.y = determ3(m.x.x, m.y.x, m.z.x, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w);
	result.w.z = determ3(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.w, m.y.w, m.z.w);
	result.w.w = determ3(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z);

	return result;
}

mat4 matCofactor(mat4 m){
	mat4 result;

	result.x.x = m.x.x;
	result.y.x = -1*(m.y.x);
	result.z.x = m.z.x;
	result.w.x = -1*(m.w.x);

	result.x.y = -1*(m.x.y);
	result.y.y = (m.y.y);
	result.z.y = -1*(m.z.y);
	result.w.y = (m.w.y);

	result.x.z = m.x.z;
	result.y.z = -1*(m.y.z);
	result.z.z = m.z.z;
	result.w.z = -1*(m.w.z);

	result.x.w = -1*(m.x.w);
	result.y.w = (m.y.w);
	result.z.w = -1*(m.z.w);
	result.w.w = (m.w.w);

	return result;
}

mat4 matInv(mat4 m){
	mat4 minor = matMinor(m);
	mat4 cofactor = matCofactor(minor);
	mat4 transposed = matTranspose(cofactor);
	GLfloat determinant = determ4(m, minor);
	mat4 result = matScalar((1/determinant), transposed);
	return result;
}

vec4 matVec(mat4 m, vec4 v){
	vec4 result;
	result.x = (m.x.x * v.x) + (m.y.x * v.y) + (m.z.x * v.z) + (m.w.x * v.w);
	result.y = (m.x.y * v.x) + (m.y.y * v.y) + (m.z.y * v.z) + (m.w.y * v.w);
	result.z = (m.x.z * v.x) + (m.y.z * v.y) + (m.z.z * v.z) + (m.w.z * v.w);
	result.w = (m.x.w * v.x) + (m.y.w * v.y) + (m.z.w * v.z) + (m.w.w * v.w);
	return result;
}

mat4 translate(GLfloat x, GLfloat y, GLfloat z){
	mat4 result;

	result.x.x = 1; result.y.x = 0; result.z.x = 0; result.w.x = x;
	result.x.y = 0; result.y.y = 1; result.z.y = 0; result.w.y = y;
	result.x.z = 0; result.y.z = 0; result.z.z = 1; result.w.z = z;
	result.x.w = 0; result.y.w = 0; result.z.w = 0; result.w.w = 1;

	return result;
}

mat4 scale(GLfloat x, GLfloat y, GLfloat z){
	mat4 result;

	result.x.x = x; result.y.x = 0; result.z.x = 0; result.w.x = 0;
	result.x.y = 0; result.y.y = y; result.z.y = 0; result.w.y = 0;
	result.x.z = 0; result.y.z = 0; result.z.z = z; result.w.z = 0;
	result.x.w = 0; result.y.w = 0; result.z.w = 0; result.w.w = 1;

	return result;
}

mat4 rotate_x(GLfloat theta){
	mat4 result;
	GLfloat s = (M_PI/180)*theta;

	result.x.x = 1; result.y.x = 0; result.z.x = 0; result.w.x = 0;
	result.x.y = 0; result.y.y = cos(s); result.z.y = -sin(s); result.w.y = 0;
	result.x.z = 0; result.y.z = sin(s); result.z.z = cos(s); result.w.z = 0;
	result.x.w = 0; result.y.w = 0; result.z.w = 0; result.w.w = 1;

	return result;
}

mat4 rotate_y(GLfloat theta){
	mat4 result;
	GLfloat s = (M_PI/180)*theta;

	result.x.x = cos(s); result.y.x = 0; result.z.x = sin(s); result.w.x = 0;
	result.x.y = 0; result.y.y = 1; result.z.y = 0; result.w.y = 0;
	result.x.z = -sin(s); result.y.z = 0; result.z.z = cos(s); result.w.z = 0;
	result.x.w = 0; result.y.w = 0; result.z.w = 0; result.w.w = 1;

	return result;
}

mat4 rotate_z(GLfloat theta){
	mat4 result;
	GLfloat s = (M_PI/180)*theta;

	result.x.x = cos(s); result.y.x = -sin(s); result.z.x = 0; result.w.x = 0;
	result.x.y = sin(s); result.y.y = cos(s); result.z.y = 0; result.w.y = 0;
	result.x.z = 0; result.y.z = 0; result.z.z = 1; result.w.z = 0;
	result.x.w = 0; result.y.w = 0; result.z.w = 0; result.w.w = 1;

	return result;
}

//returns the magnitude of a vector
GLfloat getMagnitude (vec4 vec){
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

mat4 matIdentity(){
	mat4 ident = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
	return ident;
}

mat4 rotateAboutVector(vec4 u, GLfloat theta){
    GLfloat uMag;
    GLfloat d;
    mat4 identity = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

    uMag = getMagnitude(u); //uMag is the magnitude of vector u

    vec4 unit = {u.x/uMag, u.y/uMag, u.z/uMag, u.w}; //unit vector of u (magnitude of 1)

    GLfloat unitMag = getMagnitude(unit);
    if(!(unitMag > .98 && unitMag < 1.02) ) return identity;
    d = sqrt( (unit.y*unit.y) + (unit.z*unit.z) ); //magnitude of the projection of u onto plane x=0

    // matrix for rotating vector to plane y=0
    mat4 xRotation1;
    if(unit.y == 0.0 && unit.z == 0.0){
        xRotation1.x.x = 1; xRotation1.y.x = 0;         xRotation1.z.x = 0;         xRotation1.w.x = 0;
        xRotation1.x.y = 0; xRotation1.y.y = 1;         xRotation1.z.y = 0;         xRotation1.w.y = 0;
        xRotation1.x.z = 0; xRotation1.y.z = 0;         xRotation1.z.z = 1;         xRotation1.w.z = 0;
        xRotation1.x.w = 0; xRotation1.y.w = 0;         xRotation1.z.w = 0;         xRotation1.w.w = 1;
    }
    else{
        xRotation1.x.x = 1; xRotation1.y.x = 0;           xRotation1.z.x = 0;           xRotation1.w.x = 0;
        xRotation1.x.y = 0; xRotation1.y.y = unit.z/d;  xRotation1.z.y = -(unit.y/d);   xRotation1.w.y = 0;
        xRotation1.x.z = 0; xRotation1.y.z = unit.y/d;  xRotation1.z.z = unit.z/d;      xRotation1.w.z = 0;
        xRotation1.x.w = 0; xRotation1.y.w = 0;           xRotation1.z.w = 0;           xRotation1.w.w = 1;
    }

    // matrix for rotating vector to plane x=0
    mat4 yRotation1;
    yRotation1.x.x = d;          yRotation1.y.x = 0;  yRotation1.z.x = -unit.x;      yRotation1.w.x = 0;
    yRotation1.x.y = 0;          yRotation1.y.y = 1;  yRotation1.z.y = 0;              yRotation1.w.y = 0;
    yRotation1.x.z = unit.x;   yRotation1.y.z = 0;  yRotation1.z.z = d;              yRotation1.w.z = 0;
    yRotation1.x.w = 0;          yRotation1.y.w = 0;  yRotation1.z.w = 0;              yRotation1.w.w = 1;


    mat4 zRotation = rotate_z(theta); //rotate the object the given number of degrees


    mat4 xRotation2;
    if(unit.y == 0.0 && unit.z == 0.0){
        xRotation2.x.x = 1; xRotation2.y.x = 0;         xRotation2.z.x = 0;         xRotation2.w.x = 0;
        xRotation2.x.y = 0; xRotation2.y.y = 1;         xRotation2.z.y = 0;         xRotation2.w.y = 0;
        xRotation2.x.z = 0; xRotation2.y.z = 0;         xRotation2.z.z = 1;         xRotation2.w.z = 0;
        xRotation2.x.w = 0; xRotation2.y.w = 0;         xRotation2.z.w = 0;         xRotation2.w.w = 1;
    }
    else{
        xRotation2.x.x = 1; xRotation2.y.x = 0;           xRotation2.z.x = 0;              xRotation2.w.x = 0;
        xRotation2.x.y = 0; xRotation2.y.y = unit.z/d;  xRotation2.z.y = (unit.y/d);  xRotation2.w.y = 0;
        xRotation2.x.z = 0; xRotation2.y.z = -unit.y/d;  xRotation2.z.z = (unit.z/d);   xRotation2.w.z = 0;
        xRotation2.x.w = 0; xRotation2.y.w = 0;           xRotation2.z.w = 0;              xRotation2.w.w = 1;
    }

    mat4 yRotation2;
    yRotation2.x.x = d;              yRotation2.y.x = 0;  yRotation2.z.x = unit.x;      yRotation2.w.x = 0;
    yRotation2.x.y = 0;              yRotation2.y.y = 1;      yRotation2.z.y = 0;              yRotation2.w.y = 0;
    yRotation2.x.z = -unit.x;      yRotation2.y.z = 0;     yRotation2.z.z = d;              yRotation2.w.z = 0;
    yRotation2.x.w = 0;              yRotation2.y.w = 0;      yRotation2.z.w = 0;              yRotation2.w.w = 1;

    mat4 m1 = matMult(yRotation1, xRotation1);
    mat4 m2 = matMult(zRotation, m1);
    mat4 m3 = matMult(yRotation2, m2);
    mat4 result = matMult(xRotation2, m3);

    return result;
}

mat4 look_at(vec4 eye, vec4 at, vec4 up)
{
	mat4 m;
	vec4 n, u, v;

	n = vecSub(eye, at);
	GLfloat nMag = getMagnitude(n);
	n = vecScalar(1/nMag, n);

	//if(up.x == n.x && up.y == n.y && up.z == n.z){ //if looking straight down
	//	u = (vec4){1, 0, 0, 0};
	//}
	//else{
		u = vecCross(up, n);
		GLfloat uMag = getMagnitude(u);
		u = vecScalar(1/uMag, u);
	//}


	v = vecCross(n, u);
	GLfloat vMag = getMagnitude(v);
	v = vecScalar(1/vMag, v);

	m.x.x = u.x;	m.y.x = u.y;	m.z.x = u.z;	m.w.x = 0;
	m.x.y = v.x;	m.y.y = v.y;	m.z.y = v.z;	m.w.y = 0;
	m.x.z = n.x;	m.y.z = n.y;	m.z.z = n.z;	m.w.z = 0;
	m.x.w = eye.x;	m.y.w = eye.y;	m.z.w = eye.z;	m.w.w = 1;

	m = matTranspose(m);
	m = matInv(m);
	return m;
}



mat4 ortho(GLfloat left, GLfloat right,
			GLfloat bottom, GLfloat top,
			GLfloat near, GLfloat far)
{
	mat4 result;

	result.x.x = 2/(right-left);	result.y.x = 0;					result.z.x = 0;				result.w.x = -((right+left)/(right-left));
	result.x.y = 0;					result.y.y = 2/(top-bottom);	result.z.y = 0;				result.w.y = -((top+bottom)/(top-bottom));
	result.x.z = 0;					result.y.z = 0;					result.z.z = 2/(near-far);	result.w.z = -((near+far)/(near-far));
	result.x.w = 0;					result.y.w = 0;					result.z.w = 0;				result.w.w = 1;

	return result;
}


mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom,
				GLfloat top, GLfloat near, GLfloat far)
{
	mat4 result;

	result.x.x = (-2*near)/(right-left);	result.y.x = 0;							result.z.x = (left+right)/(right-left);		result.w.x = 0;
	result.x.y = 0;							result.y.y = (-2*near)/(top-bottom);	result.z.y = (bottom+top)/(top-bottom);		result.w.y = 0;
	result.x.z = 0;							result.y.z = 0;							result.z.z = (near+far)/(far-near);			result.w.z = -(2*near*far)/(far-near);
	result.x.w = 0;							result.y.w = 0;							result.z.w = -1;							result.w.w = 0;

	return result;
}