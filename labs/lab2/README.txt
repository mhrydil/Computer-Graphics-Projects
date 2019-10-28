Matthew Hrydil mbh38@pitt.edu
CS1566 Lab 2
Submitted Friday, September 13, 2019

Contents:
matLib.c
matLib.h
matLib.o
test.c
makefile
README.txt

Execution: To run test.c, first enter the command: 'make test', then 
run the test program my entering the command: './test'



matLib.h
________

header file containing the structs that will be used to represent 4x1 vectors
and 4x4 matrices.

Also includes the method signatures for the vector and matrix functions:


//Vector functions
void printVec(vec4 vec); //takes vec4 as param, prints the vec4 to the console
vec4 vecScalar(float s, vec4 vec); //takes a float and vec4, returns a vec4 which is the result of scalar multiplication.
vec4 vecAdd(vec4 v1, vec4 v2); // takes 2 vec4s(v1 and v2), returns a vec4(v1+v2)
vec4 vecSub(vec4 v1, vec4 v2); // takes 2 vec4s(v1 and v2), returns a vec4(v1-v2)
float vecDot(vec4 v1, vec4 v2); //takes 2 vec4s(v1 and v2), returns a float (v1 (dot) v2)
vec4 vecCross(vec4 v1, vec4 v2); //takes 2 vec4s(v1 and v2), returns a vec4(v1xv2)


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

matLib.c
________

C file containing the implementation of the mentions mentioned in the header.

function 'mat4 matInv(mat4 m)' calculates the inverse of m in multiple steps by calculating the minor matrix of m, the cofactor of minor, transposing the cofactor, then multiplying it by 1/determinant(m).


test.c
______

test client to test the functionality of matLib.c. This includes the methods outlined in lab02_vector_matrix.pdf as well as the tests that were requested when demoing the lab to the TA.


Does your program run without error?

Yes.

If not, what is/are the error(s) and which parts of your program run correctly?:
