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
//#include "lab3.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices = 108;
vec4 vertices[108];
vec4 colors[108];

GLuint ctm_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

vec4 twinCube_location = {0.0, 0.5, 0.0, 1.0};
vec4 leftCube_location = {-0.5, -0.5, 0.0, 1.0};
vec4 rightCube_location = {0.5, -0.5, 0.0, 1.0};
GLfloat twinCube_degree = 0.0, leftCube_degree = 0.0, rightCube_degree = 0.0;
mat4 twinCube_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 leftCube_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 rightCube_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

// makes three squares
void fillEdges(vec4* vert, int numVertices, float t){
    vert[0] = (vec4){-.5, -.5, .5, 1};
    vert[1] = (vec4){.5, -.5, .5, 1};
    vert[2] = (vec4){.5, .5, .5, 1};
    vert[3] = (vec4){-.5, -.5, .5, 1};
    vert[4] = (vec4){.5, .5, .5, 1};
    vert[5] = (vec4){-.5, .5, .5, 1};

    for(int i=0; i<6; i++){ //rotate about y 90 degrees (right wall)
        vert[6+i] = matVec(rotate_y(90), vert[i]);
    }
    for(int i=0; i<6; i++){ //rotate about y 180 degrees (back wall)
        vert[12+i] = matVec(rotate_y(180), vert[i]);
    }
    for(int i=0; i<6; i++){ //rotate about y 270 degrees  (left wall)
        vert[18+i] = matVec(rotate_y(270), vert[i]);
    }
    for(int i=0; i<6; i++){ //rotate about x 90 degrees (bottom wall)
        vert[24+i] = matVec(rotate_x(90), vert[i]);
    }
    for(int i=0; i<6; i++){ //rotate about x 270 degrees (top wall)
        vert[30+i] = matVec(rotate_x(270), vert[i]);
    }

    for(int i=0; i<108; i++){
        vert[i] = vert[i%36];
    }


}


//fills the triangles with random colors.
void fillColors(vec4* colors, int size){
    for (int i=0; i<size/6; i++)
    {
        float randx = ((double) rand() / (RAND_MAX));
        float randy = ((double) rand() / (RAND_MAX));
        float randz = ((double) rand() / (RAND_MAX));
        vec4 curr = {randx, randy, randz, 1};
        vec4 next = {randx, randy, randz, 1};
        vec4 tip = {randx, randy, randz, 1};

        colors[i*6+1] = curr;
        colors[i*6] = next;
        colors[i*6+2] = tip;
        colors[i*6+4] = curr;
        colors[i*6+3] = next;
        colors[i*6+5] = tip;
    }
}


void init(void)
{
    GLuint program = initShader("vshader_ctm.glsl", "fshader_ctm.glsl");
    glUseProgram(program);

    ctm_location = glGetUniformLocation(program, "ctm");

    for(int i=0; i<36; i++){
        vertices[i] = matVec(scale(.5, .5, .5), vertices[i]);
        vertices[i] = matVec(translate(-.5, .5, 0), vertices[i]);
    }
    for(int i=36; i<72; i++){
        vertices[i] = matVec(scale(.5, .5, .5), vertices[i]);
        vertices[i] = matVec(translate(.5, .5, 0), vertices[i]);
    }

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
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat*) &twinCube_ctm);
    glDrawArrays(GL_TRIANGLES, 0, 72);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat*) &leftCube_ctm);
    glDrawArrays(GL_TRIANGLES, 72, 36);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat*) &rightCube_ctm);
    glDrawArrays(GL_TRIANGLES, 72, 36);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    // glDrawArrays(GL_TRIANGLES, 0, num_vertices);

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

    if(twinCube_degree == 360){
        twinCube_degree = 0;
    }
    else{
        twinCube_degree += 2;
    }

    if(leftCube_degree == 360){
        leftCube_degree = 0;
    }
    else{
        leftCube_degree += 2;
    }

    if(rightCube_degree == 360){
        rightCube_degree = 0;
    }
    else{
        rightCube_degree += 2;
    }

    leftCube_ctm = scale(.5, .5, .5);
    leftCube_ctm = matMult(rotate_z(leftCube_degree), leftCube_ctm);    
    leftCube_ctm = matMult(translate(leftCube_location.x, leftCube_location.y, 0), leftCube_ctm);

    rightCube_ctm = scale(.5, .5, .5);
    rightCube_ctm = matMult(rotate_x(rightCube_degree), rightCube_ctm);
    rightCube_ctm = matMult(translate(rightCube_location.x, rightCube_location. y, 0), rightCube_ctm);

    twinCube_ctm = rotate_y(twinCube_degree);

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    
    int num_vertices = 108;
    fillEdges(vertices, num_vertices, 1);
    fillColors(colors, num_vertices);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Cone");
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
 
