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
``
#endif  // __APPLE__

#include "initShader.h"
#include <math.h>
#include "../../common/matLib.h"
#include <stdio.h>
#include <time.h>
//#include "lab3.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef struct{
    int nWall;
    int eWall;
    int sWall;
    int wWall;
} cell;

cell maze[8][8];
int num_vertices = 5760;
// vec4 pillars[2916];
// vec2 tex_pillars[2916];
vec4 vertices[5760];
vec4 colors[5760];
vec2 tex_coords[5760];
vec4 eye;
vec4 at;
vec4 up;

GLuint ctm_location;
GLuint model_view_location;
GLuint projection_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 projection = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};;
mat4 model_view = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 identity = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};


int windowSize;
vec4 downClick;

void printMaze(int size);
void printCell(int x, int y){
    printf("north: %d\n", maze[x][y].nWall);
    printf("east: %d\n", maze[x][y].eWall);
    printf("south: %d\n", maze[x][y].sWall);
    printf("west: %d\n", maze[x][y].wWall);
}

int randomNum(int from, int to){ //returns a random number betweeen from and to inclusive
    if(to == from) return -1;
    return rand() % (to-from);
}

void drawWestWalls(int r, int c, int rMin, int rMax){
    for(int i=rMin; i<rMax; i++){
        maze[i][c].wWall = 1;
    }
}

void drawNorthWalls(int r, int c, int cMin, int cMax){
    for(int i=cMin; i<cMax; i++){
        maze[r][i].nWall = 1;
    }
}


void generateMazeRec(int rMax, int rMin, int cMax, int cMin){
    int rDiff = rMax - rMin;
    int cDiff = cMax - cMin;
    if(rDiff <= 1 || cDiff <= 1) return; //base case the row or column is only one cell wide, so we can't put any walls in it
    else{
        // printf("rMax: %d rMin: %d cMax: %d cMin: %d\n", rMax, rMin, cMax, cMin);
        int rRand = 1+randomNum(rMin+1, rMax); // returns random number from rMin+1 to rMax
        int cRand = 1+randomNum(cMin+1, cMax); // returns a random number from cMin+1 to cMax
        int r = rMin + rRand; //this is the cell that we randomly selected to draw walls from
        int c = cMin + cRand;
        // printf("row: %d col: %d\n", r, c);
        drawNorthWalls(r, c, cMin, cMax);
        drawWestWalls(r, c, rMin, rMax);
        // printMaze(8);


        int randomWall = randomNum(0, 3);
        
        //remove 1 wall from each set of walls except the random wall selected
        if(randomWall != 0){
            int x = randomNum(rMin, r-1);
            if(x == -1){
                maze[rMin][c].wWall = 0;
            }
            else maze[(x+rMin)][c].wWall = 0;
            //printf("removing west wall of cell [%d][%d]\n", x+rMin, c);
        }
        if(randomWall != 1){
            int x = randomNum(cMin, c-1);
            if(x == -1){
                maze[r][cMin].nWall = 0;
            }
            else maze[r][(x+cMin)].nWall = 0;
            //printf("removing north wall of cell [%d][%d]\n", r, x+cMin);
        }
        if(randomWall != 2){
            int x = randomNum(0, rMax-r);
            if(x == -1){
                maze[r][c].wWall = 0;
            }
            else maze[(x+r)][c].wWall = 0;
            // printf("removing west wall of cell [%d][%d]\n", x+r, c);
        }
        if(randomWall != 3){
            int x = randomNum(0, cMax-c);
            if(x == -1){
                maze[r][c].wWall = 0;
            }
            else maze[r][(x+c)].nWall = 0;
            // printf("removing north wall of cell [%d][%d]\n", r, x+c);
        }

        // printMaze(8);
        generateMazeRec(r, rMin, c, cMin);
        generateMazeRec(rMax, r, c, cMin); 
        generateMazeRec(r, rMin, cMax, c);
        generateMazeRec(rMax, r, cMax, c);
    }
}

void generateMaze(){
    //generate the outer walls for the maze
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(i == 0) maze[i][j].nWall = 1;
            if(i==7) maze[i][j].sWall = 1;
            if((j == 0) && i != j) maze[i][j].wWall = 1;
            if(j == 7 && j != i) maze[i][j].eWall = 1;
        }
    }
    //generate the inner walls for the maze, recursively
    generateMazeRec(8, 0, 8, 0);

}



void printMaze(int size){
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(maze[i][j].nWall) printf("+---");
            else printf("+   ");
        }
        printf("+\n");
        for(int j=0; j<size; j++){
            if(maze[i][j].wWall || maze[i][j].eWall){
                if(maze[i][j].wWall && maze[i][j].eWall) printf("|   |");
                else if(maze[i][j].wWall) printf("|   ");
                else printf("    |");
            }
            else{
                printf("    ");
            }
        }
        printf("\n");
    }
    for(int j=0; j<8; j++){
        if(maze[7][j].sWall) printf("+---");
        else printf("+   ");
    }
    printf("+\n");
}

void makeCubes(vec4* vert, int numVertices, float t){
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

    // scale by 5 on the y-axis to make the pillars
    for(int i=0; i<36; i++){
    	vert[i] = matVec(scale(1, 5, 1), vert[i]);
    }
    int currVert = 0;

    //puts a pillar at each of the locations
    for(int c=0; c<9; c++){
    	for(int r=0; r<9; r++){
    		for(int i=0; i<36; i++){
    			vert[currVert++] = matVec(translate(5*c, 0, 5*r), vert[i]);
    		}
    	}
    }

    //scale the pillar to make a wall
    for(int i=0; i<8; i++){
        for(int j=0; j<36; j++){
            vert[currVert] = matVec(scale(5, .8, .8), vert[j]);
            vert[currVert] = matVec(translate(5*i + 2.5, -.5, -.1), vert[currVert]);
            currVert++;
        }
    }
    for(int i=0; i<8; i++){
        for(int j=0; j<36; j++){
            vert[currVert] = matVec(scale(5, .8, .8), vert[j]);
            vert[currVert] = matVec(translate(5*i + 2.5, -.5, 40+.1), vert[currVert]);
            currVert++;
        }
    }
    for(int i=1; i<8; i++){
        for(int j=0; j<36; j++){
            vert[currVert] = matVec(scale(5, .8, .8), vert[j]);
            vert[currVert] = matVec(rotate_y(90), vert[currVert]);
            vert[currVert] = matVec(translate(0, -.5, 5*i+2.6), vert[currVert]);
            currVert++;
        }
    }

    for(int i=0; i<7; i++){
        for(int j=0; j<36; j++){
            vert[currVert] = matVec(scale(5, .8, .8), vert[j]);
            vert[currVert] = matVec(rotate_y(90), vert[currVert]);
            vert[currVert] = matVec(translate(40, -.5, 5*i+2.6), vert[currVert]);
            currVert++;
        }
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

void fillTextures(vec2* textures, int size){
    textures[0] = (vec2){.5, .5};
    textures[1] = (vec2){1, .5};
    textures[2] = (vec2){1, 0.0};
    textures[3] = (vec2){.5, 0.5};
    textures[4] = (vec2){1, 0};
    textures[5] = (vec2){0.5, 0.0};
    for(int i=0; i<30; i++){
        textures[6+i] = textures[i%6];
    }
    for(int i=0; i<2916; i++){
    	textures[i] = textures[i%36];
    }

    textures[2916] = (vec2){0, .5};
    textures[2917] = (vec2){.5, .5};
    textures[2918] = (vec2){.5, 0};
    textures[2919] = (vec2){0, .5};
    textures[2920] = (vec2){.5, 0};
    textures[2921] = (vec2){0, 0};
    for(int i=2916; i<num_vertices; i++){
        textures[i] = textures[2916 + i%6];
    }

}


void init(void)
{

    //glEnable(GL_DEPTH_TEST);

    int width = 800;
    int height = 800;
    GLubyte myTexels[width][height][3];

    FILE *fp = fopen("p2texture04.raw", "r");
    if(fp == NULL){
        printf("Uhoh!");
    }
    fread (myTexels, width*height*3, 1, fp);
    fclose(fp);


    GLuint program = initShader("vshader_ctm.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, myTexels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(tex_coords), tex_coords);

    GLuint tex_loc;
    tex_loc = glGetUniformLocation(program, "texture");
    glUniform1i(tex_loc, 0);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices)+sizeof(colors));



    glEnable(GL_CULL_FACE);
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

    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
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
    vec4 origin = {0, 0, 0, 1};

    vec4 tail = vecSub(downClick, origin);
    vec4 curr = getXYZ(x, y);
    vec4 head = vecSub(curr, origin);
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
		case 101: {
            eye.z -= .1; 
            at.z -= .1;
            break;
        }
		case 102: {
            eye.x += .1; 
            at.x += .1;
            break;
        }
		case 103: {
            eye.z += .1; 
            at.z += .1;
            break;
        }
		case 100: {
            eye.x -= .1; 
            at.x -= .1;
            break;
        }
	}

	model_view = look_at(eye, at, up);
	glutPostRedisplay();	
}


int main(int argc, char **argv)
{
    srand(time(NULL));
    windowSize = 800;
    int num_vertices = 5760;
    makeCubes(vertices, num_vertices, 1);
    fillColors(colors, num_vertices);
    fillTextures(tex_coords, num_vertices);
    generateMaze();
    int countWalls = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(maze[i][j].nWall) countWalls++;
            if(maze[i][j].eWall) countWalls++;
            if(maze[i][j].sWall) countWalls++;
            if(maze[i][j].wWall) countWalls++;
        }
    }
    printMaze(8);
    printf("number of walls: %d\n", countWalls);
    // for(int i=0; i<36; i++){
    //     vertices[i] = matVec(translate(1, 0, 0), vertices[i]);
    //     vertices[i] = matVec(scale(5, 5, 1), vertices[i]);
    // }
    // for(int i=36; i<num_vertices; i++){
    // 	vertices[i] = matVec(translate(-1, 0, 0), vertices[i]);
    // 	vertices[i] = matVec(scale(5, 5, 1), vertices[i]);
    // }

    // ctm = translate(0, 0, 5);
    eye = (vec4){20, 10, 20, 0};
    at = (vec4){20, 0, 20, 0};
    up = (vec4){0, 1, 0, 0};

    model_view = look_at(eye, at, up);
    // printMat(model_view);
    projection = ortho(-20, 20, -20, 20, -20, 20);
    // projection = frustum(-10, 10, -10, 10, 10, -10);

    // printMat(projection);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Cube");
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
 
