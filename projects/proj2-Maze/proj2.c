/*
 *  proj2.c
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

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef struct{
    int nWall;
    int eWall;
    int sWall;
    int wWall;
    int isUsed;
} cell;

typedef enum
{
    STILL,
    FLYING_AROUND,
    FLYING_DOWN,
    WALK_FORWARD,
    TURN_LEFT,
    TURN_RIGHT,
    SOLVING_MAZE,
    FINISH_MAZE,
} state;

typedef enum
{
    EAST,
    SOUTH,
    WEST,
    NORTH,
} direction;

state currentState = STILL;
int currIndex = 0;
int currentStep = 0;
int maxSteps = 0;
int gameMode;
vec4 origEye;
vec4 origAt;
vec4 newEye;
vec4 newAt;
vec4 changeEye;
vec4 changeAt;
direction directionFacing = EAST;

int mazeSize = 8;
cell maze[8][8];
int solution[64];
int solved;
int animationDone;
int sCounter;
int num_vertices = 28260;
vec4 vertices[28260];
vec4 colors[28260];
vec2 tex_coords[28260];
vec4 eye;
vec4 at;
vec4 up;


GLuint ctm_location;
GLuint model_view_location;
GLuint projection_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 projection = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 model_view = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 identity = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};


int windowSize;
vec4 downClick;

void printMaze(int size);

// helper function for debugging maze generation
void printCell(int x, int y){
    printf("north: %d\n", maze[x][y].nWall);
    printf("east: %d\n", maze[x][y].eWall);
    printf("south: %d\n", maze[x][y].sWall);
    printf("west: %d\n", maze[x][y].wWall);
}

//returns a random number betweeen from and to inclusive
int randomNum(int from, int to){ 
    if(to == from) return -1;
    return rand() % (to-from);
}


//helper functions for the generation of the maze
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

// recursive function that generates the inner walls for the maze at random
void generateMazeRec(int rMax, int rMin, int cMax, int cMin){
    int rDiff = rMax - rMin;
    int cDiff = cMax - cMin;
    if(rDiff <= 1 || cDiff <= 1) return; //base case the row or column is only one cell wide, so we can't put any walls in it
    else{
        int rRand = 1+randomNum(rMin+1, rMax); // returns random number from rMin+1 to rMax
        int cRand = 1+randomNum(cMin+1, cMax); // returns a random number from cMin+1 to cMax
        int r = rMin + rRand;                  //this is the cell that we randomly selected to draw walls from
        int c = cMin + cRand;
        drawNorthWalls(r, c, cMin, cMax);
        drawWestWalls(r, c, rMin, rMax);


        int randomWall = randomNum(0, 3);

        //remove 1 wall from each set of walls except the random wall selected
        if(randomWall != 0){
            int x = randomNum(rMin, r-1);
            if(x == -1){
                maze[rMin][c].wWall = 0;
            }
            else maze[(x+rMin)][c].wWall = 0;
        }
        if(randomWall != 1){
            int x = randomNum(cMin, c-1);
            if(x == -1){
                maze[r][cMin].nWall = 0;
            }
            else maze[r][(x+cMin)].nWall = 0;
        }
        if(randomWall != 2){
            int x = randomNum(0, rMax-r);
            if(x == -1){
                maze[r][c].wWall = 0;
            }
            else maze[(x+r)][c].wWall = 0;
        }
        if(randomWall != 3){
            int x = randomNum(0, cMax-c);
            if(x == -1){
                maze[r][c].wWall = 0;
            }
            else maze[r][(x+c)].nWall = 0;
        }

        generateMazeRec(r, rMin, c, cMin);
        generateMazeRec(rMax, r, c, cMin); 
        generateMazeRec(r, rMin, cMax, c);
        generateMazeRec(rMax, r, cMax, c);
    }
}
// Builds outer walls for the maze
void generateMaze(size){
    //generate the outer walls for the maze
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(i == 0) maze[i][j].nWall = 1;
            if(i==size-1) maze[i][j].sWall = 1;
            if((j == 0) && i != j) maze[i][j].wWall = 1;
            if(j == size-1 && j != i) maze[i][j].eWall = 1;
        }
    }
    //generate the inner walls for the maze, recursively
    generateMazeRec(size, 0, size, 0);
}

// helper functions for solveMaze function
int canGoEast(int row, int col){
    if(col == 7) return 0;
    if(maze[row][col].eWall) return 0;
    if(maze[row][col+1].wWall) return 0;
    if(maze[row][col+1].isUsed) return 0;
    return 1;
}
int canGoWest(int row, int col){
    if(col == 0) return 0;
    if(maze[row][col].wWall) return 0;
    if(maze[row][col-1].eWall) return 0;
    if(maze[row][col-1].isUsed) return 0;
    return 1;
}
int canGoNorth(int row, int col){
    if(row == 0) return 0;
    if(maze[row][col].nWall) return 0;
    if(maze[row-1][col].sWall) return 0;
    if(maze[row-1][col].isUsed) return 0;
    return 1;
}
int canGoSouth(int row, int col){
    if(row == 7) return 0;
    if(maze[row][col].sWall) return 0;
    if(maze[row+1][col].nWall) return 0;
    if(maze[row+1][col].isUsed) return 0;
    return 1;
}

// recursive backtracking algorithm to solve the maze. Stores the sequence of moves in the array solution
void solveMaze(int row, int col){
    if(row == 7 && col == 7){
        solved = 1; //base case
    }


    if(!solved && canGoEast(row, col)){
        solution[sCounter++] = 1;
        maze[row][col].isUsed = 1;
        solveMaze(row, col+1);
        if(!solved){
            solution[--sCounter] = 0;
        }
    }

    if(!solved && canGoSouth(row, col)){
        solution[sCounter++] = 2;
        maze[row][col].isUsed = 1;
        solveMaze(row+1, col);
        if(!solved){
            solution[--sCounter] = 0;
        }
    }

    if(!solved && canGoWest(row, col)){
        solution[sCounter++] = 3;
        maze[row][col].isUsed = 1;
        solveMaze(row, col-1);
        if(!solved){
            solution[--sCounter] = 0;
        }
    }

    if(!solved && canGoNorth(row, col)){
        solution[sCounter++] = 4;
        maze[row][col].isUsed = 1;
        solveMaze(row-1, col);
        if(!solved){
            solution[--sCounter] = 0;
        }
    }
}

//prints the solution to the maze to the console
void printSolution(){
    int count = 0;
    while(solution[count] != 0){
        printf("%d ", solution[count++]);
    }
}

// prints the maze to console
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
    for(int j=0; j<size; j++){
        if(maze[size-1][j].sWall) printf("+---");
        else printf("+   ");
    }
    printf("+\n");
}

// builds a cube at the address passed in
void buildCube(vec4* start){
    start[0] = (vec4){-.5, -.5, .5, 1};
    start[1] = (vec4){.5, -.5, .5, 1};
    start[2] = (vec4){.5, .5, .5, 1};
    start[3] = (vec4){-.5, -.5, .5, 1};
    start[4] = (vec4){.5, .5, .5, 1};
    start[5] = (vec4){-.5, .5, .5, 1};
    for(int i=0; i<6; i++){ //rotate about y 90 degrees (right wall)
        start[6+i] = matVec(rotate_y(90), start[i]);
    }
    for(int i=0; i<6; i++){ //rotate about y 180 degrees (back wall)
        start[12+i] = matVec(rotate_y(180), start[i]);
    }
    for(int i=0; i<6; i++){ //rotate about y 270 degrees  (left wall)
        start[18+i] = matVec(rotate_y(270), start[i]);
    }
    for(int i=0; i<6; i++){ //rotate about x 90 degrees (bottom wall)
        start[24+i] = matVec(rotate_x(90), start[i]);
    }
    for(int i=0; i<6; i++){ //rotate about x 270 degrees (top wall)
        start[30+i] = matVec(rotate_x(270), start[i]);
    }
}
//Helper methods for the makeScene function
void buildNorthWall(vec4* start){
    buildCube(start);
    for(int i=0; i<36; i++){
        start[i] = matVec(scale(5, 4, .5), start[i]);
        start[i] = matVec(translate(2.5, -.5, 0), start[i]);
        start[i] = matVec(rotate_y(90), start[i]);
    }
}
void buildEastWall(vec4* start){
    buildCube(start);
    for(int i=0; i<36; i++){
        start[i] = matVec(scale(5, 4, .5), start[i]);
        start[i] = matVec(translate(2.5, -.5, -5), start[i]);
    }
}
void buildSouthWall(vec4* start){
    buildCube(start);
    for(int i=0; i<36; i++){
        start[i] = matVec(scale(5, 4, .5), start[i]);
        start[i] = matVec(translate(2.5, -.5, 5), start[i]);
        start[i] = matVec(rotate_y(90), start[i]);
    }
}
void buildWestWall(vec4* start){
    buildCube(start);
    for(int i=0; i<36; i++){
        start[i] = matVec(scale(5, 4, .5), start[i]);
        start[i] = matVec(translate(2.5, -.5, 0), start[i]);
    }
}

// First, create a cube and scale it on y axis to represent a pillar, copy it to all locations
// Next, based on the 2d array of cells(maze variable) creates all of the walls
// Finally, creates the ground
void makeScene(vec4* vert, int numVertices, float t){
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
    			vert[currVert++] = matVec(translate(5*c, 0, -5*r), vert[i]);
    		}
    	}
    }

    //build the walls based on the maze
    for(int i=0; i<mazeSize; i++){
        for(int j=0; j<mazeSize; j++){
            int startVertex = currVert;
            if(maze[i][j].nWall){
               buildNorthWall(&vert[currVert]);
               currVert += 36;
            }
            if(maze[i][j].eWall){
               buildEastWall(&vert[currVert]);
               currVert += 36;
            }
            if(maze[i][j].sWall){
                buildSouthWall(&vert[currVert]);
                currVert += 36;
            }
            if(maze[i][j].wWall){
                buildWestWall(&vert[currVert]);
                currVert += 36;
            }
            int changedVertices = currVert-startVertex;
            for(int k=0; k<changedVertices; k++){
                vert[k+startVertex] = matVec(translate(5*i, 0, -5*j), vert[k+startVertex]);
            }
        }
    }

    // creates a single block of grass
    for(int i = 5760; i<num_vertices; i++){
        vertices[i] = matVec(scale(5, .2, 5), vertices[i%36]);
        vertices[i] = matVec(translate(-42.5, -3, 42.5), vertices[i]);
    }

    // creates a 25x25 plot of grass
    currVert = 5760;
    for(int c=0; c<25; c++){
        for(int r=0; r<25; r++){
            for(int i=0; i<36; i++){
                vert[currVert++] = matVec(translate(5*c, 0, -5*r), vert[5760+i]);
            }
        }
    }
}

// fills the cubes with the textures for what that cube represents(pillar, wall, ground)
void fillTextures(vec2* textures, int size){
    // Applies stone texture to pillars
    textures[0] = (vec2){.5, .5};
    textures[1] = (vec2){.7, .5};
    textures[2] = (vec2){.7, 0.0};
    textures[3] = (vec2){.5, 0.5};
    textures[4] = (vec2){.7, 0};
    textures[5] = (vec2){0.5, 0.0};
    for(int i=0; i<2916; i++){
    	textures[i] = textures[i%6];
    }

    // Applies brick texture to walls
    textures[2916] = (vec2){0, .5};
    textures[2917] = (vec2){.5, .5};
    textures[2918] = (vec2){.5, 0};
    textures[2919] = (vec2){0, .5};
    textures[2920] = (vec2){.5, 0};
    textures[2921] = (vec2){0, 0};
    for(int i=2916; i<num_vertices; i++){
        textures[i] = textures[2916 + i%6];
    }

    // Applies grass texture to ground cubes
    textures[5760] = (vec2){0, 1};
    textures[5761] = (vec2){.5, 1};
    textures[5762] = (vec2){.5, 0.5};
    textures[5763] = (vec2){0, 1};
    textures[5764] = (vec2){.5, .5};
    textures[5765] = (vec2){0, 0.5};
    for(int i = 5760; i<num_vertices; i++){
        textures[i] = textures[5760 + i%6];
    }

}

vec4 getXYZ (int x, int y){ //Only really used for rotation using mouse, not essential to this assignment, but can be helpful when viewing the maze from above
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


void init(void)
{
    int width = 800;
    int height = 800;
    GLubyte myTexels[width][height][3];

    FILE *fp = fopen("p2texture04_2.raw", "r");
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
    glClearColor(.529, .808, .922, 1);
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
    if(key == 'p'){
        gameMode = 1;
        currentState = FLYING_AROUND;
    }
    if(key == 'a'){
        gameMode = 0;
        currentState = FLYING_AROUND;
    }

    //glutPostRedisplay();
}

// not used for this, but can be used to rotate maze about origin by uncommenting the motion function in main
void mouse(int button, int state, int x, int y){
    if(button == 0){
        if(state == 0){
            downClick = getXYZ(x,y);
        }
    }
    // glutPostRedisplay();
}

// not used for this, but can be used to rotate maze about origin by uncommenting the motion function in main
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

// Arrow keys assigned for navigating through maze in play mode
void special(int key, int x, int y){
	switch(key){
        case 100: { //left arrow key
            currentState = TURN_LEFT;
            break;
        }
		case 101: { //up arrow key
            currentState = WALK_FORWARD;
            break;
        }
		case 102: { //right arrow key
            currentState = TURN_RIGHT;
            break;
        }
	}

	model_view = look_at(eye, at, up);
	glutPostRedisplay();	
}

// Animation! (see readme.md)
void idle(void){
    if(currentState == FLYING_AROUND){
        eye = matVec(rotate_y(2), eye);
        currentStep++;
        if(currentStep == maxSteps+1){
            currentState = FLYING_DOWN;
            maxSteps = 50;
            currentStep = 0;
            origEye = eye;
            origAt = at;
        }
    }

    if(currentState == FLYING_DOWN){
        newEye = (vec4){-17.5, 0, 22.5, 0};
        newAt = (vec4){-17.5, 0, 17.5, 0};
        GLfloat alpha = (float) currentStep/maxSteps;
        changeEye = vecSub(newEye, origEye);
        changeAt = vecSub(newAt, origAt);
        eye = vecAdd(vecScalar(alpha, changeEye), origEye);
        at = vecAdd(vecScalar(alpha, changeAt), origAt);
        currentStep++;

        if(currentStep == maxSteps+1){
            currentState = WALK_FORWARD; // enter the maze
            currentStep = 0;
            maxSteps = 25;
            origEye = eye;
            origAt = at;
        }
    }

    if(currentState == WALK_FORWARD){
        switch(directionFacing){
            case EAST: 
                newEye = vecAdd((vec4){0, 0, -5, 0}, origEye);
                newAt = vecAdd((vec4){0, 0, -5, 0}, origAt);
                break;
            case SOUTH:
                newEye = vecAdd((vec4){5, 0, 0, 0}, origEye);
                newAt = vecAdd((vec4){5, 0, 0, 0}, origAt);
                break;
            case WEST:
                newEye = vecAdd((vec4){0, 0, 5, 0}, origEye);
                newAt = vecAdd((vec4){0, 0, 5, 0}, origAt);
                break;
            case NORTH:
                newEye = vecAdd((vec4){-5, 0, 0, 0}, origEye);
                newAt = vecAdd((vec4){-5, 0, 0, 0}, origAt);
                break;
            }
            
        GLfloat alpha = (float) currentStep/maxSteps;
        changeEye = vecSub(newEye, origEye);
        changeAt = vecSub(newAt, origAt);
        eye = vecAdd(vecScalar(alpha, changeEye), origEye);
        at = vecAdd(vecScalar(alpha, changeAt), origAt);
        currentStep++;

        if(currentStep == maxSteps+1){
            if(gameMode) currentState = STILL;
            else if(!animationDone) currentState = SOLVING_MAZE;
            else currentState = STILL;
            currentStep = 0;
            maxSteps = 25;
            origEye = eye;
            origAt = at;
        }
    }

    if(currentState == TURN_RIGHT){
        switch(directionFacing){
            case EAST:
                newAt = vecAdd((vec4){5, 0, 5, 0}, origAt);
                break;
            case SOUTH:
                newAt = vecAdd((vec4){-5, 0, 5, 0}, origAt);
                break;
            case WEST:
                newAt = vecAdd((vec4){-5, 0, -5, 0}, origAt);
                break;
            case NORTH:
                newAt = vecAdd((vec4){5, 0, -5, 0}, origAt);
                break;
        }

        GLfloat alpha = (float) currentStep/maxSteps;
        changeAt = vecSub(newAt, origAt);
        at = vecAdd(vecScalar(alpha, changeAt), origAt);
        currentStep++;

        if(currentStep == maxSteps+1){
            switch(directionFacing){
                case EAST:
                    directionFacing = SOUTH;
                    break;
                case SOUTH:
                    directionFacing = WEST;
                    break;
                case WEST:
                    directionFacing = NORTH;
                    break;
                case NORTH:
                    directionFacing = EAST;
                    break;
            }
            if(gameMode) currentState = STILL;
            else currentState = SOLVING_MAZE;
            currentStep = 0;
            maxSteps = 25;
            origAt = at;
        }
    }

    if(currentState == TURN_LEFT){
        switch(directionFacing){
            case EAST:
                newAt = vecAdd((vec4){-5, 0, 5, 0}, origAt);
                break;
            case SOUTH:
                newAt = vecAdd((vec4){-5, 0, -5, 0}, origAt);
                break;
            case WEST:
                newAt = vecAdd((vec4){5, 0, -5, 0}, origAt);
                break;
            case NORTH:
                newAt = vecAdd((vec4){5, 0, 5, 0}, origAt);
                break;
        }

        GLfloat alpha = (float) currentStep/maxSteps;
        changeAt = vecSub(newAt, origAt);
        at = vecAdd(vecScalar(alpha, changeAt), origAt);
        currentStep++;

        if(currentStep == maxSteps+1){
            switch(directionFacing){
                case EAST:
                    directionFacing = NORTH;
                    break;
                case SOUTH:
                    directionFacing = EAST;
                    break;
                case WEST:
                    directionFacing = SOUTH;
                    break;
                case NORTH:
                    directionFacing = WEST;
                    break;
            }
            if(gameMode) currentState = STILL;
            else currentState = SOLVING_MAZE;
            currentStep = 0;
            maxSteps = 25;
            origAt = at;
        }
    }

    if(currentState == SOLVING_MAZE){
        if(solution[currIndex] == 0){
            animationDone = 1;
            if(directionFacing == EAST) currentState = WALK_FORWARD;
            if(directionFacing == SOUTH) currentState = TURN_LEFT;
        }
        else if(directionFacing == solution[currIndex]-1){
            currentState = WALK_FORWARD;
            currIndex++;
        }
        else{
            switch(solution[currIndex]-1){
                case EAST:
                    if(directionFacing == NORTH) currentState = TURN_RIGHT;
                    if(directionFacing == SOUTH) currentState = TURN_LEFT; break;
                case SOUTH:
                    if(directionFacing == EAST) currentState = TURN_RIGHT;
                    if(directionFacing == WEST) currentState = TURN_LEFT; break;
                case WEST:
                    if(directionFacing == SOUTH) currentState = TURN_RIGHT;
                    if(directionFacing == NORTH) currentState = TURN_LEFT; break;
                case NORTH:
                    if(directionFacing == WEST) currentState = TURN_RIGHT;
                    if(directionFacing == EAST) currentState = TURN_LEFT; break;
            }
        }
    }

    model_view = look_at(eye, at, up);
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    srand(time(NULL)); //sets the seed for random based on the time to ensure randomly generated mazes
    windowSize = 800;
    int num_vertices = 28260;
    int mazeSize = 8;
    generateMaze(mazeSize);
    makeScene(vertices, num_vertices, 1);
    //fillColors(colors, num_vertices);
    fillTextures(tex_coords, num_vertices);
    int countWalls = 0;
    for(int i=0; i<mazeSize; i++){
        for(int j=0; j<mazeSize; j++){
            if(maze[i][j].nWall) countWalls++;
            if(maze[i][j].eWall) countWalls++;
            if(maze[i][j].sWall) countWalls++;
            if(maze[i][j].wWall) countWalls++;
        }
    }
    printMaze(mazeSize);
    printf("number of walls: %d\n", countWalls);
    solveMaze(0,0);
    printSolution();
    for(int i=0; i<num_vertices; i++){
        vertices[i] = matVec(translate(-20, 0, 20), vertices[i]);
    }

    eye = (vec4){-30, 30, 30, 0};
    at = (vec4){0, 0, 0, 0};
    up = (vec4){0, 1, 0, 0};
    model_view = look_at(eye, at, up);
    //projection = ortho(-25, 25, -25, 25, 25, -25);
    projection = frustum(-1, 1, -1, 1, -1, -150);
    // printMat(projection);

    maxSteps = 180;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Maze");
    //glewInit();
    init();
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    // glutMouseFunc(mouse);
    // glutMotionFunc(motion);
    glutSpecialFunc(special);
    glutMainLoop();
    
    return 0;
}
 