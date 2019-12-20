/*
 *  proj3.c
 *
 *  Created: Fall 2019
 *  Author: Matthew Hrydil
 *  Class:  Introduction to Computer Graphics
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
GLuint projection_location;
GLuint model_view_location;
GLuint LightPosition_location;
// GLuint EyePosition_location;
GLuint ap_location;
GLuint dp_location;
GLuint sp_location;
GLuint ac_location;
GLuint al_location;
GLuint aq_location;
GLuint shininess_location;
GLuint isShadow_location;
vec4 eye, at, up;
vec4 initialEye = {0, 0, 10, 1};
vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
// int currDegree = -30; // used to stop the rotation about the x-axis from happening when it is at either bound
int isAnimating = 0;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 projection = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 model_view = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 identity = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

mat4 red_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 green_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 blue_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 yellow_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 orange_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 light_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 table_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
int windowSize;
GLfloat radius = 10;
GLfloat xRotation = 0;
GLfloat yRotation = 0;

// Lighting

vec4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
vec4 light_specular = {1.0, 1.0, 1.0, 1.0};
vec4 light_ambient = {0.35, 0.35, 0.35, 1.0};
vec4 LightPosition = {0.0, 3.0, 0.0, 1.0};

GLfloat att_const = 1;
GLfloat att_linear = .05;
GLfloat att_quad = .002;


typedef enum material_name
{
    RED_BALL,
    GREEN_BALL,
    BLUE_BALL,
    YELLOW_BALL,
    ORANGE_BALL,
    LIGHT,
    TABLE,
    NUM_MATERIALS
} material_name;

typedef struct
{
    vec4 reflect_ambient;
    vec4 reflect_diffuse;
    vec4 reflect_specular;
    GLfloat shininess;
} material;

material materials[NUM_MATERIALS] = {
    {{1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10}, // red ball
    {{0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10}, //green ball
    {{0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10}, //blue ball
    {{1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10}, // yellow ball
    {{1.0, 0.5, 0.0, 1.0}, {1.0, 0.5, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10}, // orange ball
    {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 1}, // light
    {{0.0, 0.55, 0.0, 1.0}, {0.0, 0.55, 0.0, 1.0}, {0.0, 0.0, 0.0, 1.0}, 1} // table
};




void fillEdges(vec4* vert, int numVertices, float t){
    
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
        vert[3888 + i] = matVec(scale(20, .1, 20), vert[3888+i]);
        vert[3888 + i] = matVec(translate(0, -.05, 0), vert[3888+i]);
    }
}

// fills the normals array with the normal vector for each vertex
void fillNormals(vec4* normals, int size){
    for(int i=0; i<3888; i++){
        normals[i] = vecSub(vertices[i], (vec4){0, 0, 0, 1});
    }
    for(int i=3888/3; i<size/3; i++){
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


//fills the triangles with random colors.
// void fillColors(vec4* colors, int size){

//     for(int i=0; i<36; i++){
//         colors[3888 + i] = (vec4){0, .3, 0, 1};
//     }
//     for(int i=0; i<3888; i++){
//         colors[i] = (vec4){.25, 0, .25, 1};
//     }
// }

void calculateEye(){
    eye = matVec(rotate_x(xRotation), initialEye);
    eye = matVec(rotate_y(yRotation), eye);    
}

void init(void)
{

    fillEdges(vertices, num_vertices, 1);
    fillNormals(normals, num_vertices);
    // fillColors(colors, num_vertices);
    red_ctm = matMult(scale(.5, .5, .5), red_ctm);
    red_ctm = matMult(translate(0, .5, 0), red_ctm);
    green_ctm = matMult(scale(.5, .5, .5), green_ctm);
    green_ctm = matMult(translate(1, .5, 0), green_ctm);
    blue_ctm = matMult(scale(.5, .5, .5), blue_ctm);
    blue_ctm = matMult(translate(2, .5, 0), blue_ctm);
    yellow_ctm = matMult(scale(.5, .5, .5), yellow_ctm);
    yellow_ctm = matMult(translate(3, .5, 0), yellow_ctm);
    orange_ctm = matMult(scale(.5, .5, .5), orange_ctm);
    orange_ctm = matMult(translate(4, .5, 0), orange_ctm);
    light_ctm = matMult(scale(.3, .3, .3), light_ctm);
    light_ctm = matMult(translate(0, 3, 0), light_ctm);
    calculateEye();
    // eye = matVec(rotate_x(-30), eye);
    at = (vec4){0, 0, 0, 1};
    up = (vec4){0, 1, 0, 1};
    model_view = look_at(eye, at, up);
    projection = frustum(-1, 1, -1, 1, -1, -40);


    GLuint program = initShader("vshader_ctm.glsl", "fshader.glsl");
    glUseProgram(program);

    ctm_location = glGetUniformLocation(program, "ctm");
    projection_location = glGetUniformLocation(program, "projection");
    model_view_location = glGetUniformLocation(program, "model_view");
    isShadow_location = glGetUniformLocation(program, "isShadow");
    ap_location = glGetUniformLocation(program, "AmbientProduct");
    dp_location = glGetUniformLocation(program, "DiffuseProduct");
    sp_location = glGetUniformLocation(program, "SpecularProduct");
    shininess_location = glGetUniformLocation(program, "shininess");
    LightPosition_location = glGetUniformLocation(program, "LightPosition");
    // EyePosition_location = glGetUniformLocation(program, "EyePosition");
    ac_location = glGetUniformLocation(program, "attenuation_constant");
    al_location = glGetUniformLocation(program, "attenuation_linear");
    aq_location = glGetUniformLocation(program, "attenuation_quadratic");


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(normals));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1fv(ac_location, 1, (GLfloat *) &att_const);
    glUniform1fv(al_location, 1, (GLfloat *) &att_linear);
    glUniform1fv(aq_location, 1, (GLfloat *) &att_quad);    

    for(int i=0; i<NUM_MATERIALS; i++){
        AmbientProduct = product(materials[i].reflect_ambient, light_ambient);
        DiffuseProduct = product(materials[i].reflect_diffuse, light_diffuse);
        SpecularProduct = product(materials[i].reflect_specular, light_specular);
        glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
        glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
        glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
        glUniform1fv(shininess_location, 1, (GLfloat *) &materials[i].shininess);
        // glUniform4fv(EyePosition_location, 1, (GLfloat *) &eye);
        glUniform4fv(LightPosition_location, 1, (GLfloat *) &LightPosition);
        switch(i){
            case RED_BALL:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &red_ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                glUniform1i(isShadow_location, 1);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                break;
            case GREEN_BALL:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &green_ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                glUniform1i(isShadow_location, 1);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                break;
            case BLUE_BALL:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &blue_ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                glUniform1i(isShadow_location, 1);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                break;
            case YELLOW_BALL:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &yellow_ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                glUniform1i(isShadow_location, 1);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                break;
            case ORANGE_BALL:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &orange_ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                glUniform1i(isShadow_location, 1);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                break;
            case LIGHT:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &light_ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 0, 3888);
                break;
            case TABLE:
                glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
                glUniform1i(isShadow_location, 0);
                glDrawArrays(GL_TRIANGLES, 3888, 36);
                break;
        }
    }
    


    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection);
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view);


    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    switch(key){
        case 'q':
            exit(0);
        case 'a':
            isAnimating = 1;
            break;
        case 'x':
            isAnimating = 0;
            break;
        case 'u':
            light_ctm = matMult(translate(0, 1, 0), light_ctm);
            LightPosition.y += 1;
            glutPostRedisplay();
            break;
        case 'd':
            light_ctm = matMult(translate(0, -1, 0), light_ctm);
            LightPosition.y -= 1;
            glutPostRedisplay();
            break;
        case 'l':
            light_ctm = matMult(translate(-1, 0, 0), light_ctm);
            LightPosition.x -= 1;
            glutPostRedisplay();
            break;
        case 'r':
            light_ctm = matMult(translate(1, 0, 0), light_ctm);
            LightPosition.x += 1;
            glutPostRedisplay();
            break;
        case 'f':
            light_ctm = matMult(translate(0, 0, 1), light_ctm);
            LightPosition.z += 1;
            glutPostRedisplay();
            break;
        case 'b':
            light_ctm = matMult(translate(0, 0, -1), light_ctm);
            LightPosition.z -= 1;
            glutPostRedisplay();
            break;
        case 'i':
            if(initialEye.z != 1){
                initialEye.z -= .1;
                calculateEye();
                model_view = look_at(eye, at, up);
            }
            glutPostRedisplay();
            break;
        case 'o':
            initialEye.z += .1;
            calculateEye();
            model_view = look_at(eye, at, up);
            glutPostRedisplay();
            break;
    }

    printVec(LightPosition);
    //glutPostRedisplay();
}


void mouse(int button, int state, int x, int y){
    glutPostRedisplay();
}

void motion(int x, int y){
    glutPostRedisplay();
}

void special(int key, int x, int y){
	switch(key){
        case 100: // left
            yRotation -= 5;
            calculateEye();
            model_view = look_at(eye, at, up);
            break;
		case 101: // up
            if(xRotation != -85){
                xRotation -= 5;
                calculateEye();
                model_view = look_at(eye, at, up);
            }
            break;
        case 102: // right
            yRotation += 5;
            calculateEye();
            model_view = look_at(eye, at, up);
            break;
		case 103: // down
            if(xRotation < 0){
                xRotation += 5;
                calculateEye();
                model_view = look_at(eye, at, up);
            }
            break;
	}
	glutPostRedisplay();	
}

void idle(void){
    if(isAnimating){
        green_ctm = matMult(rotate_y(2), green_ctm);
        blue_ctm = matMult(rotate_y(4), blue_ctm);
        yellow_ctm = matMult(rotate_y(3), yellow_ctm);
        orange_ctm = matMult(rotate_y(8), orange_ctm);
    }
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    windowSize = 512;
    num_vertices = 3924;



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(50,500);
    glutCreateWindow("Project 3");
    //glewInit();
    init();
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(special);
    glutMainLoop();
    
    return 0;
}
 
