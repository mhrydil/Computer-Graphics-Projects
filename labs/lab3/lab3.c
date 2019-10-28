#include <stdio.h>
#include <math.h>
#include "lab3.h">
vec4 vertices[2160];
vec4 colors[2160];
int num_vertices = 2160;

void fillEdges(vec4* vert, int size){
    
    //Creates the edges for the top part of the cone
    for (int i=0; i<360; i++)
    {
        vec4 curr;
        vec4 next;
        vec4 tip = {0, 1, 0, 1};
        
        curr.x = cos((M_PI * i)/180);
        curr.y = 0;
        curr.z = sin((M_PI * i)/180);
        curr.w = 1;
        
        next.x = cos((M_PI * (i+1))/180);
        next.y = 0;
        next.z = sin((M_PI * (i+1))/180);
        next.w = 1;
        
        vert[i*3] = next;
        vert[i*3+1] = curr;
        vert[i*3+2] = tip;
    }
    
    //Generates the edges for the base of the cone
    for(int i=360; i<720; i++){
        vec4 curr;
        vec4 next;
        vec4 origin = {0, 0, 0, 1};
        
        curr.x = cos((M_PI * i)/180);
        curr.y = 0;
        curr.z = sin((M_PI * i)/180);
        curr.w = 1;
        
        next.x = cos((M_PI * (i+1))/180);
        next.y = 0;
        next.z = sin((M_PI * (i+1))/180);
        next.w = 1;
        
        vert[i*3] = next;
        vert[i*3+1] = curr;
        vert[i*3+2] = origin;
    }
}

//fills the triangles with random colors.
void fillColors(vec4* colors, int size){
    for (int i=0; i<720; i++)
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