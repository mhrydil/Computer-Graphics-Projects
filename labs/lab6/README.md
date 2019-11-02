# Lab6
## Matthew Hrydil
## CS1566

**Compilation:**
- I have a common folder which contains my matLib.c, matLib.h and matLib.o files. lab6.c uses a relative reference to this folder to access these files. I did this to avoid copying/pasting the matLib file into every different project for this class. As a result, my submission includes two folders: common and labs. The labs folder contains a subfolder to lab6 where the code for this lab exists.
- You can compile the file using the command 'make lab6' and the file can be run with the command './lab6'.

**The Program:**
Everything in this file works as expected. There is an array of vertices[108] which holds the vertices for 3 cubes, each made up of 36 vertices. The first two cubes are scaled, translated, and rotated using the idle() function. The final square is drawn twic in the display() function after being transformed by two different transformation matrices, which are adjusted in the idle() function.  

Animation is achieved in the idle() function by changing the degree of rotation for each of the different transformation matrices. The rotation is incremented by two degrees each time the idle function is called. When it reaches 360 degrees, it resets the degree to 0 and continues the rotation.