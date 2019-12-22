Matthew Hrydil
mbh38
CS1566 - Project 1

* I use a mac

Files included in submission:
- torus.c
- Makefile
- fshader.glsl
- initShader.c
- initShader.h
- matLib.c
- matLib.h
- matLib.o
- vshasder_ctm.glsl

The file can be run using the gcc command outlined in the makefile, or by simply typing 'make torus'. It can be run using the ./torus command.

The file torus.c includes functions for zooming in and out and rotating about an arbitrary axis. Zooming in and out can be achieved by using the up and down arrow keys. Rotation is achieved by clicking and dragging the screen.

torus.c functions:

fillEdges - creates the vertices of the torus and adds them to the array of vertices

fillColors - creates random colors and adds them to an array for each of the triangles in the shape

getXYZ - returns the 3d coordinate based on where the screen was clicked.

motion - 
	calculates the vector orthogonal to the one based on the mouse click/drag.

	generates the motion about an arbitrary axis by calling the rotateAboutVector method in my matLib library.

	updates a global variable called downClick which keeps track of where the mouse was the last time the method was called.

special - scales the object up or down depending on the keys pressed (up or 	down arrow keys)