## Introduction to Computer Graphics
## Matthew Hrydil
## Fall 2019

***

## Project 4 - Rubik's Cube

### Usage:

- This project can be compiled by using the 'make' command in terminal.
- The program can be run using the command './proj4'
- The program can be stopped by pressing the 'q' button when the program is running.

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

- The code for this project is split up between 4 primary files:
	- matLib.c (and matLib.h)
	- proj4.c
	- vshader_ctm.glsl
	- solve_rc.c (and solve_rc.h)
		- **These files were provided by the instructor and provide the code necessary to determine the solution to a rubik's cube.**

- **matLib.c**
	- matrix library containing a number of functions that operate on vectors and matrices.

- **proj4.c**
	- **note:** *A "cubie" refers to the individual cube (of which there are 27) that make up a Rubik's cube.*  
	- **Global Variables**
		- I used several global variables to keep track of the state of different things for this project. I have an overall "ctm" matrix which is used to track the overall rotation of the cube and the zoom level.
		- There are 6 arrays (front, right, back, left, up, down) which are used to keep track of the location of each individual cubie as faces of the cube are rotated.
			- These are initialized to show which cubie occupies each location for each face.
			- Initially, the top left cubie in the front face is cubie 0, the top middle is cubie 1, etc... so front[0] = 0, front[1] = 1, etc... The 
			- Each corner cubie is shared by three faces at any given time, and each non corner edge cubie is shared by 2 faces at any given time. The center cubie is only included in one face at any given time.
			- When a face is rotated, the array for each face changes to reflect which cubie occupies each index in that array.
		- the cubies[27] array of matrices keeps track of the transformation matrix for each cubie.
		- For animation, there are several variables used to track the state of animation.
	- **Creating the cube**
		- First I built an individual "cubie" with the beveled edges in the "fillEdges" function. I manually generated the vertices for a single face with the beveled edge and corner, and then rotated that several times to create the entire cubie.
		- in the init function, I set the transformation matrix for each cubie to scale and move each of the cubies to their correct starting position.
		- The display function first sends the overall CTM(current transformation matrix) to the graphcis card. Then, for each of the cubies, it sets the colors for that cubie, draws it, and sends it to the graphics card along with its CTM.
	- **Rotating a face**
		- When a face of a Rubik's cube changes, the position of each cubie on that face changes, as well as the cubies on each face that shares an edge with the face that has been rotated. (For example, if you rotate the front face, you affect the top, right, bottom, and left face in addition to the front face).
		- The location of each cubie must be tracked when a face is rotated. So when the rotateFront() function is called, the cubie number that occupies a location in the front face is copied to a temporary array, then the front array is changed to hold the correct cubie number at each index. The arrays that represent the faces that border the front face are then adjusted to reflect which cubies are in the correct indices for that face as well.
		- After all the edges have been adjusted, the "isRotating" flag is set to 1 for the face that was rotated, this allows the animation to occur.
		- Only one rotation is allowed to occur at a time to prevent the cube from being messed up. If a rotation was attempted, but something was currently being rotated, the requested rotation will not work, and the rotate function will return 0. If it is successful, it will return 1.
	- **Shuffling the cube**
		- When the shuffle function is called(by pressing the 's' key), a sequence of 30 random moves will be generated and will fill the randomSequence global array with characters representing the different possible moves.
		- Any time a move is made when shuffling, a call will be made to a function in solve_rc.c which tells that file about the move that was made.
	- **Solving the cube**
		- By calling the solve_rc() method in the solve_rc.c file, we are able to obtain a string of characters which represent the moves needed to solve the cube. The format of this string is different than the format I used, so I first convert the format of the string to work with my program and store the string of moves in the solution[] array.
		- At the end of the solve() method, I set the isSolving tag to 1 so that the animation can occur.
	- **Keyboard input**
		- Keyboard input is used to rotate any face, shuffle the cube and solve the cube.
			- When any key is pressed, I call the function for that method. For the rotate functions, they will return true if there was no face being moved at the time the call was made. If the rotate function returns true, I notified the solve_rc file of the move. This is necessary to keep track of the state of the Rubik's cube so that it can solve the cube.
			- Shuffling the cube simply calls the shuffle function, and solving it simply calls the solve fiunction.
	- **Animation**
		- The idle function is where the animation occurs, and it is vital to making this program work correctly.
		- At any given time, only one animation flag will be true. (ie. the program won't be rotating more than one face at a time.)
		- if a given flag is true, it will perform the animation necessary to rotate the cube to its correct location.
		- Solving and shuffling use a "currIndex" variable to track where they are at in the array of moves that need to be performed. If the program is shuffling or solving, it will look at the current index in that array of characters, then call the function to rotate that face. Because only one face can be rotated at a time, we check to see if the move was successful before moving to the next move.
		- When we reach the end of either the shuffle or solve arrays, we perform that move and then set the flag to false to stop the shuffling or solving.

- **vShader_ctm.glsl**
	- This file contains the basic lighting model and it sends the position and color of each vertex to the graphics card.

- **solve_rc.c**
	- This file contains the code to keep track of the state of the Rubik's cube and generate a solution to the cube. It was provided by the professor.