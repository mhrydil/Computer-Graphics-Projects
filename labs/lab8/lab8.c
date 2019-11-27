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

int num_vertices = 3924;
vec4 vertices[3924];
vec4 normals[3924];
vec4 colors[3924];
GLuint ctm_location;
GLuint model_view_location;
GLuint light_pos_location;
GLuint is_shadow_location;
GLuint projection;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 projection = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 model_view_matrix = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
GLfloat x_value = 0;
int isGoingRight = 1;
int windowSize;
int spinning = 0;

vec4 prev;
vec4 downClick;
vec4 upClick;


void fillEdges(vec4* vert, int numVertices, float t){
    
    GLfloat nextYCoord;
    GLfloat theta;
    GLfloat phi;
    GLfloat theta2;
    GLfloat phi2;
    vec4 a;
    vec4 b;
    vec4 c;
    vec4 d;

    float sectionSize = 10;

    for(int i = 0; i<9; i++){
        theta = 0; // 0, 10, 20, 30, 40, 50, 60, 70, 80
        theta2 = 10*(M_PI/180); // 10, 20, 30, 40, 50, 60, 70, 80
        phi = (90 - (i*sectionSize))*(M_PI/180); //Z-angle
        phi2 = (90 - (i+1)*sectionSize)*(M_PI/180);
        a.x = sin(theta)*sin(phi);      a.y = cos(phi);     a.z = cos(theta) * sin(phi);    a.w = 1;
        b.x = sin(theta2)*sin(phi);     b.y = cos(phi);     b.z = cos(theta2) * sin(phi);   b.w = 1;
        c.x = sin(theta)*sin(phi2);     c.y = cos(phi2);    c.z = cos(theta) * sin(phi2);   c.w = 1;
        d.x = sin(theta2)*sin(phi2);    d.y = cos(phi2);    d.z = cos(theta2) * sin(phi2);  d.w = 1;

        vert[i*6] = a;
        vert[i*6+1] = b;
        vert[i*6+2] = c;
        vert[i*6+3] = c;
        vert[i*6+4] = b;
        vert[i*6+5] = d;
    }
    for(int i=0; i<36; i++){ //rotate the 10 degree section of vertices around the sphere 36 times
        for(int j=0; j<54; j++){
            vert[i*54 + j] = matVec(rotate_y(i*10), vert[j]);
        }
    }
    for(int i=0; i<1944; i++){
        vert[1944+i] = matVec(rotate_x(180), vert[i]);
    }
    for(int i=0; i<num_vertices; i++){
        vert[i] = matVec(scale(.25, .25, .25), vert[i]);
        vert[i] = matVec(translate(-.5, .25, 0), vert[i]);
    }

    // builds the cube
    vert[3888] = (vec4){-.5, -.5, .5, 1};
    vert[3888 + 1] = (vec4){.5, -.5, .5, 1};
    vert[3888 + 2] = (vec4){.5, .5, .5, 1};
    vert[3888 + 3] = (vec4){-.5, -.5, .5, 1};
    vert[3888 + 4] = (vec4){.5, .5, .5, 1};
    vert[3888 + 5] = (vec4){-.5, .5, .5, 1};
    for(int i=0; i<6; i++){ //rotate about y 90 degrees (right wall)
        vert[6+3888 + i] = matVec(rotate_y(90), vert[3888 + i]);
    }
    for(int i=0; i<6; i++){ //rotate about y 180 degrees (back wall)
        vert[12+3888 + i] = matVec(rotate_y(180), vert[3888 + i]);
    }
    for(int i=0; i<6; i++){ //rotate about y 270 degrees  (left wall)
        vert[18+3888 + i] = matVec(rotate_y(270), vert[3888 + i]);
    }
    for(int i=0; i<6; i++){ //rotate about x 90 degrees (bottom wall)
        vert[24+3888 + i] = matVec(rotate_x(90), vert[3888 + i]);
    }
    for(int i=0; i<6; i++){ //rotate about x 270 degrees (top wall)
        vert[30+3888 + i] = matVec(rotate_x(270), vert[3888 + i]);
    }

    for(int i=0; i<36; i++){
        vert[3888 + i] = matVec(scale(.5, .5, .5), vert[3888+i]);
        vert[3888 + i] = matVec(translate(.5, .25, 0), vert[3888+i]);
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

void fillNormals(vec4* normals, int size){
	for(int i=0; i<size/3; i++){
		vec4 first = vertices[i*3];
		vec4 second = vertices[i*3+1];
		vec4 third = vertices[i*3+2];

		vec4 v1 = vecSub(second, first);
		vec4 v2 = vecSub(third, second);
		vec4 cross = vecCross(v1, v2);
		vec4 norm = vecScalar(1/getMagnitude(cross), cross);
		normals[i*3] = norm;
		normals[i*3+1] = norm;
		normals[i*3+2] = norm;
	}
}


void init(void)
{
    GLuint program = initShader("vshader_ctm.glsl", "fshader.glsl");
    glUseProgram(program);

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view_matrix");
    light_pos_location = glGetUniformLocation(program, "light_position");
    is_shadow_location = glGetUniformLocation(program, "is_shadow");
    projection_location = glGetUniformLocation(program, "projection");

    float eye_x, eye_y, eye_z;
    float at_x, at_y, at_z;
    float light_x, light_y, light_z;
    printf("Please enter an eye point: ");
    scanf("(%f,%f,%f)", &eye_x, &eye_y, &eye_z);
    vec4 eye = (vec4){eye_x, eye_y, eye_z, 1};
    printf("Please enter an at point: ");
    scanf("(%f,%f,%f)", &at_x, &at_y, &at_z);
    vec4 at = (vec4){at_x, at_y, at_z, 1};
    printf("Please enter a light point: ");
    scanf("(%f,%f,%f)", &light_x, &light_y, &light_z);
    vec4 light = (vec4){light_x, light_y, light_z, 1};
    vec4 up = (vec4){0, 1, 0, 1};
    model_view_matrix = look_at(eye, at, up);

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
    int num_vertices = 3924;
    fillEdges(vertices, num_vertices, 1);
    fillColors(colors, num_vertices);
    fillNormals(normals, num_vertices);
    vec4 eye = {0, .1, .1, 1};
    vec4 at = {0, 0, 0, 1};
    vec4 up = {0, 1, 0, 1};
    model_view_matrix = look_at(eye, at, up);
    projection = frustum(-1, 1, -1, 1, -1, -10);

    // float eye_x, eye_y, eye_z;
    // float at_x, at_y, at_z;
    // float light_x, light_y, light_z;
    // printf("Please enter an eye point: ");
    // scanf("(%f,%f,%f)", &eye_x, &eye_y, &eye_z);


    ctm = translate(0, 0, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(50,500);
    glutCreateWindow("SPHERE");
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
 
