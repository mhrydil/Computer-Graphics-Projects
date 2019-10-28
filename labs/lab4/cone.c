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

void mouse(int button, int state, int x, int y){
	printf("Button: %d, State: %d, X: %d, Y: %d\n", button, state, x, y);

}

void motion(int x, int y){
	printf("Motion: X: %d, Y: %d\n", x, y);
}

void special(int key, int x, int y){
	printf("Special: Key: %d, X: %d, Y: %d\n", key, x, y);
	switch(key){
		case 101: ctm = matMult(scale(1.05, 1.05, 1.05), ctm); break;
		case 103: ctm = matMult(scale(1/1.05, 1/1.05, 1/1.05), ctm); break;
	}
	glutPostRedisplay();	
}

void idle(void){
    if(isGoingRight)
    {
        x_value += 0.05;
        if(fabs(x_value - 1.0) < 0.000001)
        {
            x_value = 1.0;
            isGoingRight = 0;
        }
    }
    else
    {
        x_value -= 0.05;
        if(fabs(x_value - (-1.0)) < 0.000001)
        {
            x_value = -1.0;
            isGoingRight = 1;
        }
    }

    ctm = translate(x_value, 0.0, 0.0);
    printMat(ctm);

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    
    int num_vertices = 1440;
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
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
 
