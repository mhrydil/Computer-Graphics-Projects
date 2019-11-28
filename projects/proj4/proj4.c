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

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices = 144;
int NUM_CUBIES = 27;
vec4 vertices[144];
vec4 colors[144];

GLuint ctm_location;
GLuint cubie_ctm_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 identity = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
int front[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
int right[9] = {2, 11, 20, 5, 14, 23, 8, 17, 26};
int back[9] = {20, 19, 18, 23, 22, 21, 26, 25, 24};
int left[9] = {18, 9, 0, 21, 12, 3, 24, 15, 6};
int up[9] = {18, 19, 20, 9, 10, 11, 0, 1, 2};
int down[9] = {6, 7, 8, 15, 16, 17, 24, 25, 26};
mat4 cubies[27];
int windowSize;
int spinning = 0;

vec4 downClick;


void fillEdges(vec4* vert, int numVertices, float t){
    // builds the front face of the cube
    vert[0] = (vec4){-.5, -.5, .5, 1};
    vert[1] = (vec4){.5, -.5, .5, 1};
    vert[2] = (vec4){.5, .5, .5, 1};
    vert[3] = (vec4){-.5, -.5, .5, 1};
    vert[4] = (vec4){.5, .5, .5, 1};
    vert[5] = (vec4){-.5, .5, .5, 1};
    for(int i=0; i<6; i++){
        vert[i] = matVec(translate(0, 0, .1), vert[i]);
    }
    // builds the top 45 degree angled part
    vert[6] = (vec4){-.5, .5, .6, 1};
    vert[7] = (vec4){.5, .5, .6, 1};
    vert[8] = (vec4){.5, .6, .5, 1};
    vert[9] = (vec4){-.5, .5, .6, 1};
    vert[10] = (vec4){.5, .6, .5, 1};
    vert[11] = (vec4){-.5, .6, .5, 1};
    // builds the right 45 degree angled part
    vert[12] = (vec4){.5, .5, .6, 1};
    vert[13] = (vec4){.5, -.5, .6, 1};
    vert[14] = (vec4){.6, -.5, .5, 1};
    vert[15] = (vec4){.5, .5, .6, 1};
    vert[16] = (vec4){.6, -.5, .5, 1};
    vert[17] = (vec4){.6, .5, .5, 1};
    // builds the top right triangular angled part
    vert[18] = (vec4){.5, .5, .6, 1};
    vert[19] = (vec4){.6, .5, .5, 1};
    vert[20] = (vec4){.5, .6, .5, 1};
    // builds the bottom right triangular angled part
    vert[21] = (vec4){.5, -.6, .5, 1};
    vert[22] = (vec4){.6, -.5, .5, 1};
    vert[23] = (vec4){.5, -.5, .6, 1};

    for(int i=0; i<24; i++){ //rotate about y 90 degrees (right wall)
        vert[24 + i] = matVec(rotate_y(90), vert[i]);
    }
    for(int i=0; i<24; i++){ //rotate about y 180 degrees (back wall)
        vert[48 + i] = matVec(rotate_y(180), vert[i]);
    }
    for(int i=0; i<24; i++){ //rotate about y 270 degrees  (left wall)
        vert[72 + i] = matVec(rotate_y(270), vert[i]);
    }
    for(int i=0; i<24; i++){ //rotate about x 90 degrees (bottom wall)
        vert[96 + i] = matVec(rotate_x(90), vert[i]);
    }
    for(int i=0; i<6; i++){ //rotate just the top face about x 270 degrees (top face)
        vert[120 + i] = matVec(rotate_x(270), vert[i]);
    }
    for(int i=0; i<18; i++){ // rotate just the bottom edges about y 180 degrees
        vert[126 + i] = matVec(rotate_y(180), vert[102+i]);
    }
}


//fills the triangles with random colors.
void fillColors(vec4* colors, int size){
    for (int i=0; i<size/24; i++)
    {
        float randx = ((double) rand() / (RAND_MAX));
        float randy = ((double) rand() / (RAND_MAX));
        float randz = ((double) rand() / (RAND_MAX));
        vec4 curr = {randx, randy, randz, 1};
        vec4 next = {randx, randy, randz, 1};
        vec4 tip = {randx, randy, randz, 1};

        colors[i*24+1] = curr;
        colors[i*24] = next;
        colors[i*24+2] = tip;
        colors[i*24+3] = next;
        colors[i*24+4] = curr;
        colors[i*24+5] = tip;
    }

    for(int i=0; i<size; i++){
        if(i%24 >= 6){
            colors[i] = (vec4){0.1, 0.1, 0.1, 1};
        }
    }
}


void init(void)
{

	for(int i=0; i<NUM_CUBIES; i++){
		cubies[i] = identity;
		cubies[i] = matMult(scale(.333, .333, .333), cubies[i]);
		// Z-Placement
		if(i<9){
			cubies[i] = matMult(translate(0, 0, .405), cubies[i]);
		}
		if(i>=18){
			cubies[i] = matMult(translate(0, 0, -.405), cubies[i]);
		}
		// Y-Placement
		if(i%9 == 0 || i%9 == 1 || i%9 == 2){
			cubies[i] = matMult(translate(0, .405, 0), cubies[i]);
		}
		if(i%9 == 6 || i%9 == 7 || i%9 == 8){
			cubies[i] = matMult(translate(0, -.405, 0), cubies[i]);
		}
		// X-Placement
		if(i%9 == 0 || i%9 == 3 || i%9 == 6){
			cubies[i] = matMult(translate(-.405, 0, 0), cubies[i]);
		}
		if(i%9 == 2 || i%9 == 5 || i%9 == 8){
			cubies[i] = matMult(translate(.405, 0, 0), cubies[i]);
		}

	}
    GLuint program = initShader("vshader_ctm.glsl", "fshader.glsl");
    glUseProgram(program);

    ctm_location = glGetUniformLocation(program, "ctm");
    cubie_ctm_location = glGetUniformLocation(program, "cubie_ctm");

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
    glClearColor(1.0, 1.0, 1.0, 1.0);
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

    for(int i=0; i<NUM_CUBIES; i++){
    	if(i<9){ // front face
			for(int i=0; i<6; i++){
				colors[i] = (vec4){0, .25, 0, 1};
			}
		}
		else{
			for(int i=0; i<6; i++){
				colors[i] = (vec4){0, 0, 0, 1};
			}
		}
		if(i>=18){ // back face
			for(int i=0; i<6; i++){
				colors[48+i] = (vec4){0, 0, 1, 1};
			}
		}
		else{
			for(int i=0; i<6; i++){
				colors[48+i] = (vec4){0, 0, 0, 1};
			}
		}
		// Y-Placement
		if(i%9 == 0 || i%9 == 1 || i%9 == 2){ // top face
			for(int i=0; i<6; i++){
				colors[120+i] = (vec4){1, 1, 1, 1};
			}
		}
		else{
			for(int i=0; i<6; i++){
				colors[120+i] = (vec4){0, 0, 0, 1};
			}
		}
		if(i%9 == 6 || i%9 == 7 || i%9 == 8){ // bottom face
			for(int i=0; i<6; i++){
				colors[96+i] = (vec4){1, 1, 0, 1};
			}
		}
		else{
			for(int i=0; i<6; i++){
				colors[96+i] = (vec4){0, 0, 0, 1};
			}
		}
		// X-Placement
		if(i%9 == 0 || i%9 == 3 || i%9 == 6){ // left face
			for(int i=0; i<6; i++){
				colors[72+i] = (vec4){1, .35, 0, 1};
			}
		}
		else{
			for(int i=0; i<6; i++){
				colors[72+i] = (vec4){0, 0, 0, 1};
			}
		}
		if(i%9 == 2 || i%9 == 5 || i%9 == 8){ // right face
			for(int i=0; i<6; i++){
				colors[24+i] = (vec4){.6, 0, 0, 1};
			}
		}
		else{
			for(int i=0; i<6; i++){
				colors[24+i] = (vec4){0, 0, 0, 1};
			}
		}
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    	glUniformMatrix4fv(cubie_ctm_location, 1, GL_FALSE, (GLfloat *) &cubies[i]);
    	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    }

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);
    if(key == 'f'){
    	int temp[9];
    	for(int i=0; i<9; i++){
    		temp[i] = front[i];
    	}
    	front[0] = temp[6];
    	front[1] = temp[3];
    	front[2] = temp[0];
    	front[3] = temp[7];
    	front[4] = temp[4];
    	front[5] = temp[1];
    	front[6] = temp[8];
    	front[7] = temp[5];
    	front[8] = temp[2];
    	right[0] = front[2];
    	right[3] = front[5];
    	right[6] = front[8];
    	up[6] = front[0];
    	up[7] = front[1];
    	up[8] = front[2];
    	left[2] = front[0];
    	left[5] = front[3];
    	left[8] = front[6];
    	down[0] = front[6];
    	down[1] = front[7];
    	down[2] = front[8];
    	for(int i = 0; i<9; i++){
    		cubies[front[i]] = matMult(rotate_z(-90), cubies[front[i]]);
    	}
    }
    if(key == 'r'){
    	int temp[9];
    	for(int i=0; i<9; i++){
    		temp[i] = right[i];
    	}
    	right[0] = temp[6];
    	right[1] = temp[3];
    	right[2] = temp[0];
    	right[3] = temp[7];
    	right[4] = temp[4];
    	right[5] = temp[1];
    	right[6] = temp[8];
    	right[7] = temp[5];
    	right[8] = temp[2];
    	back[0] = right[2];
    	back[3] = right[5];
    	back[6] = right[8];
    	up[2] = right[2];
    	up[5] = right[1];
    	up[8] = right[0];
    	front[2] = right[0];
    	front[5] = right[3];
    	front[8] = right[6];
    	down[2] = right[6];
    	down[5] = right[7];
    	down[8] = right[8];
    	for(int i = 0; i<9; i++){
    		cubies[right[i]] = matMult(rotate_x(-90), cubies[right[i]]);
    	}
    }
    if(key == 'u'){
    	int temp[9];
    	for(int i=0; i<9; i++){
    		temp[i] = up[i];
    	}
    	up[0] = temp[6];
    	up[1] = temp[3];
    	up[2] = temp[0];
    	up[3] = temp[7];
    	up[4] = temp[4];
    	up[5] = temp[1];
    	up[6] = temp[8];
    	up[7] = temp[5];
    	up[8] = temp[2];
    	right[0] = up[8];
    	right[1] = up[5];
    	right[2] = up[2];
    	back[0] = up[2];
    	back[1] = up[1];
    	back[2] = up[0];
    	left[0] = up[0];
    	left[1] = up[3];
    	left[2] = up[6];
    	front[0] = up[6];
    	front[1] = up[7];
    	front[2] = up[8];
    	for(int i = 0; i<9; i++){
    		cubies[up[i]] = matMult(rotate_y(-90), cubies[up[i]]);
    	}
    }
    if(key == 'l'){
    	int temp[9];
    	for(int i=0; i<9; i++){
    		temp[i] = left[i];
    	}
    	left[0] = temp[6];
    	left[1] = temp[3];
    	left[2] = temp[0];
    	left[3] = temp[7];
    	left[4] = temp[4];
    	left[5] = temp[1];
    	left[6] = temp[8];
    	left[7] = temp[5];
    	left[8] = temp[2];
    	up[0] = left[0];
    	up[3] = left[1];
    	up[6] = left[2];
    	front[0] = left[2];
    	front[3] = left[5];
    	front[6] = left[8];
    	down[6] = left[6];
    	down[3] = left[7];
    	down[0] = left[8];
    	back[2] = left[0];
    	back[5] = left[3];
    	back[8] = left[6];
    	for(int i = 0; i<9; i++){
    		cubies[left[i]] = matMult(rotate_x(90), cubies[left[i]]);
    	}
    }
    if(key == 'b'){
    	int temp[9];
    	for(int i=0; i<9; i++){
    		temp[i] = back[i];
    	}
    	back[0] = temp[6];
    	back[1] = temp[3];
    	back[2] = temp[0];
    	back[3] = temp[7];
    	back[4] = temp[4];
    	back[5] = temp[1];
    	back[6] = temp[8];
    	back[7] = temp[5];
    	back[8] = temp[2];
    	up[2] = back[0];
    	up[1] = back[1];
    	up[0] = back[2];
    	left[0] = back[2];
    	left[3] = back[5];
    	left[6] = back[8];
    	down[6] = back[8];
    	down[7] = back[7];
    	down[8] = back[6];
    	right[2] = back[0];
    	right[5] = back[3];
    	right[8] = back[6];
    	for(int i = 0; i<9; i++){
    		cubies[back[i]] = matMult(rotate_z(90), cubies[back[i]]);
    	}
    }
    if(key == 'd'){
    	int temp[9];
    	for(int i=0; i<9; i++){
    		temp[i] = down[i];
    	}
    	down[0] = temp[6];
    	down[1] = temp[3];
    	down[2] = temp[0];
    	down[3] = temp[7];
    	down[4] = temp[4];
    	down[5] = temp[1];
    	down[6] = temp[8];
    	down[7] = temp[5];
    	down[8] = temp[2];
    	front[6] = down[0];
    	front[7] = down[1];
    	front[8] = down[2];
    	right[6] = down[2];
    	right[7] = down[5];
    	right[8] = down[8];
    	back[6] = down[8];
    	back[7] = down[7];
    	back[8] = down[6];
    	left[6] = down[6];
    	left[7] = down[3];
    	left[8] = down[0];
    	for(int i = 0; i<9; i++){
    		cubies[down[i]] = matMult(rotate_y(90), cubies[down[i]]);
    	}
    }




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
    int num_vertices = 144;
    fillEdges(vertices, num_vertices, 1);
    // fillColors(colors, num_vertices);
    ctm = translate(0, 0, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200,200);
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
 
