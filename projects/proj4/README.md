# CS1566
## Matthew Hrydil
## Fall 2019

***

## Project 4

### Usage:

- This project can be compiled by using the 'make' command in terminal.
- The program can be run using the command ./proj4
- The program can be stopped by pressing the 'q' button when the program is running.

### It works!

- Functionally, this program works as it is supposed to.

### Running the program

- after using the command ./proj4, the program will begin. Initially, the front face of the rubik's cube is showing.
- The user can interact with the cube by pressing the following keys:
	- 'u' - rotate the top(upper) face
	- 'd' - rotate the lower(down) face
	- 'r' - rotate the right face
	- 'l' - rotate the left face
	- 'f' - rotate the front face
	- 'b' - rotate the back face
	- 's' - shuffles the rubik's cube (30 random moves)
	- ' ' (spacebar) - solves the rubik's cube
- the user can rotate the cube by clicking and dragging. And should they desire, they can zoom in and out using the up and down keys.

## Code

- My code for this assignment is broken up between three primary files:
	- matLib.c (and matLib.h)
	- proj4.c
	- vshader_ctm.glsl
- The solve_rc.c and solve_rc.h files were provided and provide the code necessary to determine the solution to a rubik's cube.
