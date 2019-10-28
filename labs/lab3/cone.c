/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


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
//#include "lab3.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices = 720;
vec4 vertices[720];
vec4 colors[720];


void fillEdges(vec4* vert, int numVertices, float tip){
    
    // printf("Tip of cone is at (0, %.2f, 0).\n", tip);
    // printf("Base of cone is at (0, 0, 0)\n");
    // printf("Number of vertices: %d\n", numVertices);
    // printf("Height of cone: %d\n", abs(tip));

    int numSections = numVertices/3;
    float sectionSize = 360.0/numSections;
    
    //Creates the edges for the top part of the cone
    
    GLfloat t = tip;
    for (int i=0; i<numSections/2; i++)
    {
        vec4 curr;
        vec4 next;
        vec4 tip = {0, t, 0, 1};

        curr.x = cos((M_PI * i*sectionSize)/180);
        curr.y = 0;
        curr.z = sin((M_PI * i*sectionSize)/180);
        curr.w = 1;

        next.x = cos((M_PI * (i+1)*sectionSize)/180);
        next.y = 0;
        next.z = sin((M_PI * (i+1)*sectionSize)/180);
        next.w = 1;

        vert[i*3] = next;
        vert[i*3+1] = curr;
        vert[i*3+2] = tip;
    }
    

    //Generates the edges for the base of the cone
    for(int i=numSections/2; i<numSections; i++){
        vec4 curr;
        vec4 next;
        vec4 origin = {0, 0, 0, 1};

        curr.x = cos((M_PI * i*sectionSize)/180);
        curr.y = 0;
        curr.z = sin((M_PI * i*sectionSize)/180);
        curr.w = 1;

        next.x = cos((M_PI * (i+1)*sectionSize)/180);
        next.y = 0;
        next.z = sin((M_PI * (i+1)*sectionSize)/180);
        next.w = 1;

        vert[i*3] = next;
        vert[i*3+1] = curr;
        vert[i*3+2] = origin;
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
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

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

int main(int argc, char **argv)
{
    
    int num_vertices = 720;
    fillEdges(vertices, num_vertices, 1);
    fillColors(colors, num_vertices);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Cone");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
 
