/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */

#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <stdlib.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include <math.h>
#include "matLib.h"
#include <stdio.h>
//#include "lab3.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices = 1440;
vec4 vertices[1440];
vec4 colors[1440];

GLuint ctm_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
GLfloat x_value = 0;
int isGoingRight = 1;
int windowSize;

vec4 prev;
vec4 downClick;
vec4 upClick;



void fillEdges(vec4* vert, int numVertices, float t){
    
    // printf("Tip of cone is at (0, %.2f, 0).\n", tip);
    // printf("Base of cone is at (0, 0, 0)\n");
    // printf("Number of vertices: %d\n", numVertices);
    // printf("Height of cone: %d\n", abs(tip));

    int numTriangles = numVertices/3; //480 
    int numTrianglesPerSection = numTriangles/2; //240
    float sectionSize = 360.0/numTrianglesPerSection; // 1.5
    // printf("%d ", numTriangles);
    // printf("%d ", numTrianglesPerSection);
    // printf("%.4f", sectionSize); 
    //Creates the edges for the top part of the cone
    
    vec4 tip = {0, t, 0, 1};
    for (int i=0; i<numTrianglesPerSection; i++)
    {
        vec4 curr;
        vec4 next;

        curr.x = cos((M_PI * (i*sectionSize))/180);
        curr.y = 0;
        curr.z = sin((M_PI * (i*sectionSize))/180);
        curr.w = 1;

        next.x = cos((M_PI * ((i+1)*sectionSize))/180);
        next.y = 0;
        next.z = sin((M_PI * ((i+1)*sectionSize))/180);
        next.w = 1;

        vert[i*3] = tip;
        vert[i*3+1] = next;
        vert[i*3+2] = curr;
    }
    

    //Generates the edges for the base of the cone

    vec4 origin = {0, 0, 0, 1};
    for(int i=numTrianglesPerSection; i<numTriangles; i++){
        vec4 curr;
        vec4 next;

        curr.x = cos((M_PI * i*sectionSize)/180);
        curr.y = 0;
        curr.z = sin((M_PI * i*sectionSize)/180);
        curr.w = 1;

        next.x = cos((M_PI * (i+1)*sectionSize)/180);
        next.y = 0;
        next.z = sin((M_PI * (i+1)*sectionSize)/180);
        next.w = 1;

        vert[i*3] = origin;
        vert[i*3+1] = curr;
        vert[i*3+2] = next;
    }
}


//fills the triangles with random colors.
void fillColors(vec4* colors, int size){
    for (int i=0; i<size/3; i++)
    {
        float randx = ((double) rand() / (RAND_MAX));
        float randy = ((double) rand() / (RAND_MAX));
        float randz = ((double) rand() / (RAND_MAX));
        vec4 curr = {randx, randy, randz, 1};
        vec4 next = {randx, randy, randz, 1};
        vec4 tip = {randx, randy, randz, 1};

        colors[i*3+1] = curr;
        colors[i*3] = next;
        colors[i*3+2] = tip;
    }
}


void init(void)
{
    GLuint program = initShader("vshader_ctm.glsl", "fshader.glsl");
    glUseProgram(program);

    ctm_location = glGetUniformLocation(program, "ctm");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm); //Arguments (from first to last)
                                                                    // The location (in GLuint) from the glGetUniformLocation() function
                                                                    // The number of elements (1 matrix in this case)
                                                                    // Transpose (no transpose in this case)
                                                                    // Pointer to the matrix

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

    glutPostRedisplay();
}

vec4 getXYZ (int x, int y){
    vec4 result;
    GLfloat xunit = (float)x / (float)windowSize; 
    GLfloat yunit = (float)y / (float)windowSize; 
    xunit = ((xunit - .5)) * 2; 
    yunit = ((yunit - .5)) * 2; 
    GLfloat zSquared = 1 - (xunit*xunit) - (yunit*yunit);

    result.x = xunit;
    result.y = -yunit;
    result.z = sqrt(zSquared);
    result.w = 1;

    return result;
}

GLfloat getMagnitude (vec4 vec){
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

vec4 crossVector(vec4 orig){
    vec4 zero = {0, 0, 0, 0};
    vec4 cross;
    vec4 v1 = vecSub(zero, downClick);
    vec4 v2 = vecSub(zero, orig);
    cross = vecCross(v1, v2);
    return cross;
}


//vector u is the vector we want to rotate about
mat4 rotateAboutVector(vec4 u, GLfloat theta){
	//printVec(u);
    GLfloat uMag;
    GLfloat d;
    mat4 identity = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

    uMag = getMagnitude(u); //uMag is the magnitude of vector u

    vec4 unit = {u.x/uMag, u.y/uMag, u.z/uMag, u.w}; //unit vector of u (magnitude of 1)
    // printf("unit: ");
    // printVec(unit);
    GLfloat unitMag = getMagnitude(unit);
    if(!(unitMag > .98 && unitMag < 1.02) ) return identity;
    // printf("Magnitude of Unit: %f\n", getMagnitude(unit));
    d = sqrt( (unit.y*unit.y) + (unit.z*unit.z) ); //magnitude of the projection of u onto plane x=0
    // printf("D: %f\n", d);

    // matrix for rotating vector to plane y=0
    mat4 xRotation1;
    if(unit.y < .0000001 && unit.z < .0000001){
        xRotation1.x.x = 1; xRotation1.y.x = 0;         xRotation1.z.x = 0;         xRotation1.w.x = 0;
        xRotation1.x.y = 0; xRotation1.y.y = 1;         xRotation1.z.y = 0;         xRotation1.w.y = 0;
        xRotation1.x.z = 0; xRotation1.y.z = 0;         xRotation1.z.z = 1;         xRotation1.w.z = 0;
        xRotation1.x.w = 0; xRotation1.y.w = 0;         xRotation1.z.w = 0;         xRotation1.w.w = 1;
    }
    else{
        xRotation1.x.x = 1; xRotation1.y.x = 0;           xRotation1.z.x = 0;              xRotation1.w.x = 0;
        xRotation1.x.y = 0; xRotation1.y.y = unit.z/d;  xRotation1.z.y = -(unit.y/d);  xRotation1.w.y = 0;
        xRotation1.x.z = 0; xRotation1.y.z = unit.y/d;  xRotation1.z.z = (unit.z/d);   xRotation1.w.z = 0;
        xRotation1.x.w = 0; xRotation1.y.w = 0;           xRotation1.z.w = 0;              xRotation1.w.w = 1;
        // printf("xrotation1: ");
        // printMat(xRotation1);
    }

    // matrix for rotating vector to plane x=0
    mat4 yRotation1;
    yRotation1.x.x = d;          yRotation1.y.x = 0;  yRotation1.z.x = -unit.x;      yRotation1.w.x = 0;
    yRotation1.x.y = 0;          yRotation1.y.y = 1;  yRotation1.z.y = 0;              yRotation1.w.y = 0;
    yRotation1.x.z = unit.x;   yRotation1.y.z = 0;  yRotation1.z.z = d;              yRotation1.w.z = 0;
    yRotation1.x.w = 0;          yRotation1.y.w = 0;  yRotation1.z.w = 0;              yRotation1.w.w = 1;
    // printf("yrotation1");
    // printMat(yRotation1);
    //if()
    mat4 zRotation = rotate_z(theta); //rotate the object the given number of degrees
    // printf("zRotation: ");
    // printMat(zRotation);

    mat4 xRotation2;
    if(unit.y < .0000001 && unit.z < .0000001){
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
    // printf("M1: ");
    // printMat(m1);
    mat4 m2 = matMult(zRotation, m1);
    // printf("M2: ");
    // printMat(m2);
    mat4 m3 = matMult(yRotation2, m2);
    // printf("M3: ");
    // printMat(m3);
    mat4 result = matMult(xRotation2, m3);
    // printf("Result: ");
    // printMat(result);

    return result;
}

void mouse(int button, int state, int x, int y){
	// printf("Button: %d, State: %d, X: %d, Y: %d\n", button, state, x, y);
    vec4 point = getXYZ(x, y);
    printf("XYZ: ");
    printVec(point);



    if(button == 0){
        if(state == 0){
            downClick = getXYZ(x,y);
        }
        if (state == 1){

            vec4 curr = getXYZ(x,y); //head of vector
            vec4 diff = vecSub(curr, downClick); //downclick is the tail of the vector
            // printf("head: ");
            // printVec(curr);
            // printf("Tail: ");
            // printVec(downClick);
            // printf("diff: ");
            // printVec(diff);
            GLfloat diffMag = getMagnitude(diff);
            vec4 diffUnit = vecScalar(1/diffMag, diff);
            // printf("diffUnit magnitude (should be 1): %f\n", getMagnitude(diffUnit));
            vec4 orthog = crossVector(diffUnit);
            // printf("orthog: \n");
            // printVec(orthog);
            GLfloat angle = diffMag*85;

            //ctm = matMult(rotateAboutVector(orthog, angle), ctm);
            glutPostRedisplay();
        }
    }


    //glutPostRedisplay();
}

void motion(int x, int y){
    vec4 origin = {0, 0, 0, 1};

    vec4 tail = vecSub(downClick, origin);
    printf("tail: ");
    printVec(tail);
    vec4 curr = getXYZ(x, y);
    vec4 head = vecSub(curr, origin);
    vec4 diff = vecSub(head, tail);
    printf("head: ");
    printVec(head);
    vec4 orthog = vecCross(tail, head); //vector orthogonal to both the tail and head vectors
    printf("orthog: ");
    printVec(orthog);
    printf("dot Product: %f\n", vecDot(tail, head)); 
    GLfloat dotDivideMag = (vecDot(tail, head)) / (getMagnitude(tail) * getMagnitude(head));
    GLfloat theta = acos(dotDivideMag);
    printf("dotDivideMag, %f\n", dotDivideMag);
    printf("Theta: %f\n", theta);

    //if (tail.x < 0 && head.x < 0 && tail.y < 0 && head.y < 0) theta *= -1;
    //if (tail.y <0 && head.y < 0) theta *= -1;
    //if (tail.z < 0 && head.z < 0) theta *= -1;
    ctm = matMult(rotateAboutVector(orthog, theta*180/M_PI), ctm);
    glutPostRedisplay();
    downClick = curr;

    // printf("CURR: ");
    // printVec(curr);
    // printf("Down Click: ");
    // printVec(downClick);



	//printf("Motion: X: %d, Y: %d\n", x, y);
}

void special(int key, int x, int y){
    //glutPostRedisplay();
	printf("Special: Key: %d, X: %d, Y: %d\n", key, x, y);
	switch(key){
        // case 100: ctm = matMult(ctm, rotate_y(-5)); break;
        // case 101: ctm = matMult(ctm, rotate_x(-5)); break;
        // case 102: ctm = matMult(ctm, rotate_y(5)); break;
        // case 103: ctm = matMult(ctm, rotate_x(5)); break;
		case 101: ctm = matMult(scale(1.02, 1.02, 1.02), ctm); break;
		case 103: ctm = matMult(scale(1/1.02, 1/1.02, 1/1.02), ctm); break;
	}
	glutPostRedisplay();	
}


int main(int argc, char **argv)
{
    windowSize = 512;
    int num_vertices = 1440;
    fillEdges(vertices, num_vertices, 1);
    fillColors(colors, num_vertices);
    ctm = translate(0, 0, 0);





    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Cone");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(special);
    glutMainLoop();
    
    return 0;
}
 
