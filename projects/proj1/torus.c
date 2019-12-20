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
#include "../../common/matLib.h"
#include <stdio.h>
#include <time.h>
//#include "lab3.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices = 7776;
vec4 vertices[7776];
vec4 colors[7776];

GLuint ctm_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
GLfloat x_value = 0;
int isGoingRight = 1;
int windowSize;
int spinning = 0;

vec4 prev;
vec4 downClick;
vec4 upClick;



void fillEdges(vec4* vert, int numVertices, float t){
    
    GLfloat nextYCoord;
    GLfloat theta; //theta is the angle of rotation around the z plane (0 - 2pi)
    GLfloat phi; //phi is the angle of rotation upward the plane x = 0
    GLfloat theta2;
    GLfloat phi2;
    vec4 a;
    vec4 b;
    vec4 c;
    vec4 d;

    float sectionSize = 10;

    for(int i = 0; i<36; i++){
        a.x = cos(10*i*M_PI/180); a.y = sin((10*i)*M_PI/180); a.z = 0; a.w = 1;
        a = vecScalar(.25, a);
        a.w = 1;
        a = matVec(translate(.75, 0, 0), a);
        printf("%d:\na: ", i);
        printVec(a);
        b = matVec(rotate_y(10), a);
        printf("b: ");
        printVec(b);
        c.x = cos((10*(i+1))*M_PI/180); c.y = sin((10*(i+1))*(M_PI/180)); c.z = 0;    c.w = 1;
        c = vecScalar(.25, c);
        c.w = 1;
        c = matVec(translate(.75, 0, 0), c);
        printf("c: ");
        printVec(c);
        d = matVec(rotate_y(10), c);
        printf("d: ");
        printVec(d);

        vert[i*6] = a;
        vert[i*6+1] = b;
        vert[i*6+2] = c;
        vert[i*6+3] = c;
        vert[i*6+4] = b;
        vert[i*6+5] = d;
    }

    for(int i=0; i<36; i++){ //rotate the 10 degree section of vertices around the sphere 36 times
        for(int j=0; j<216; j++){
            vert[i*216 + j] = matVec(rotate_y(i*10), vert[j]);
        }
    }
    // for(int i=0; i<1944; i++){
    //     vert[1944+i] = matVec(rotate_x(180), vert[i]);
    // }

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


void mouse(int button, int state, int x, int y){
    vec4 point = getXYZ(x, y);

    if(button == 0){
        if(state == 0){
            downClick = getXYZ(x,y);
        }
        if (state == 1){
            //mehhh
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y){
    spinning = 0;
    vec4 origin = {0, 0, 0, 1};

    vec4 tail = vecSub(downClick, origin);
    vec4 curr = getXYZ(x, y);
    vec4 head = vecSub(curr, origin);
    vec4 diff = vecSub(head, tail);
    vec4 orthog = vecCross(tail, head); //vector orthogonal to both the tail and head vectors

    GLfloat dotUnit = (vecDot(tail, head)) / (getMagnitude(tail) * getMagnitude(head));
    GLfloat theta = acos(dotUnit);

    mat4 rotation = rotateAboutVector(orthog, theta*180/M_PI);
    ctm = matMult(rotation, ctm);
    glutPostRedisplay();
    downClick = curr;
}

void special(int key, int x, int y){
	printf("Special: Key: %d, X: %d, Y: %d\n", key, x, y);
	switch(key){
		case 101: ctm = matMult(scale(1.02, 1.02, 1.02), ctm); break;
		case 103: ctm = matMult(scale(1/1.02, 1/1.02, 1/1.02), ctm); break;
	}
	glutPostRedisplay();	
}


int main(int argc, char **argv)
{
    windowSize = 512;
    int num_vertices = 7776;
    fillEdges(vertices, num_vertices, 1);
    fillColors(colors, num_vertices);
    for(int i=0; i<num_vertices; i++){
        vertices[i] = matVec(scale(1, 1, 1), vertices[i]);
    }
    ctm = translate(0, 0, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Torus");
    //glewInit();
    init();
    //glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(special);
    glutMainLoop();
    
    return 0;
}
 
