
//4x1 column vector
typedef struct{
	float x;
	float y;
	float z;
	float w;
} vec4;

//4x4 matrix, made of 4 column vectors
typedef struct{
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;


//Vector functions
void printVec(vec4 vec); //takes vec4 as param, prints the vec4 to the console
vec4 vecScalar(float s, vec4 vec); //takes a float and vec4, returns a vec4 which is the result of scalar multiplication.
vec4 vecAdd(vec4 v1, vec4 v2); // takes 2 vec4s(v1 and v2), returns a vec4(v1+v2)
vec4 vecSub(vec4 v1, vec4 v2); // takes 2 vec4s(v1 and v2), returns a vec4(v1-v2)
float vecDot(vec4 v1, vec4 v2); //takes 2 vec4s(v1 and v2), returns a float (v1 (dot) v2)
vec4 vecCross(vec4 v1, vec4 v2); //takes 2 vec4s(v1 and v2), returns a vec4(v1xv2)
float vecMag(vec4 vec); //takes a vec4(vec), returns the magnitude of vec
vec4 vecNorm(vec4 vec); //takes a vec4(vec), returns a normalized vector of vec


//Matrix functions
void printMat(mat4 m); //takes mat4 as param, prints the mat4 to the console
mat4 matTranspose(mat4 m); //takes a mat4(m), returns a mat4(the transposition of m)
mat4 matScalar(float s, mat4 mat); //takes a float and mat4, returns a mat4 which is the result of scalar multiplication.
mat4 matAdd(mat4 m1, mat4 m2); //takes 2 mat4s(m1 and m2), returns a mat4(m1+m2)
mat4 matSub(mat4 m1, mat4 m2); //takes 2 mat4s(m1 and m2), returns a mat4(m1-m2)
mat4 matMult(mat4 m1, mat4 m2); //takes 2 mat4s(m1 and m2), returns a mat4(m1*m2)
float determ3(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3); //takes 9 floats, returns the determinant
float determ4(mat4 m, mat4 minor); //takes 2 mat4s(m and minor), returns a float(the determinant of m)
mat4 matMinor(mat4 m); //takes a mat4(m), returns a mat4(the minor matrix of m)
mat4 matCofactor(mat4 m); //takes a mat4(m), returns a mat4(the cofactor of m)
mat4 matInv(mat4 m); //takes a mat4(m), returns a mat4(the inverse of m)
vec4 matVec(mat4 m, vec4 v); //takes a mat4 and vec4, returns a vec4(m * v)