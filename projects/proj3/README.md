# CS1566
## Matthew Hrydil
## Fall 2019

***

## Project 3

### Usage:

- This project can be compiled by using the 'make' command in terminal.
- The program can be run using the command ./proj3
- The program can be stopped by pressing the 'q' button when the program is running.

### Works!

- Everything in this project works as it is supposed to according to the project 3 description

### Running the program

- after using the command ./proj3, the program will begin. Initially, the eye point is (0, 0, 10). The at point throughout the program is (0, 0, 0).
- The user can begin animation by pressing the 'a' key. Animation can be stopped using the 'x' key.
- The user can move the camera throughout the scene using the arrow keys, and zooming in and out can be achieved using the 'i' and 'o' keys respectively.
- The light can be moved around the scene using the 'u', 'd', 'f', 'b', 'l' and 'r' keys.

## Code

- The code for this assignment is broken up between three primary files:
	- matLib.c (and matLib.h)
	- proj3.c
	- vshader_ctm.glsl


### matLib.c

- matLib.c has been enhanced by adding the following function:
	- `vec4 product(vec4 u, vec4 v);` - returns the product of each coordinate in u and v
		- ie. result.x = u.x * v.x, etc...
	- matLib.c contains all of the code for any of the matrix functions 

### proj3.c

- proj3.c contains all of the code to generate the objects(and their materials) in the scene, build the scene, and view/animate the scene.

- **Building the scene**
	- A single sphere and a single cube are created. The vertices for each are stored in the vec4 array 'vertices[]'
	- An array of vec4s is used to store the normal vector for each vertex in the scene.
	- The materials for each of the objects are generated manually using an array of material structs.
	- An enum of material names is used to associate each object with its material properties.
	- **the init function** 
		- fills the array of vertices as well as the array of normals.
		- scales and translates a transformation matrix for each of the objects to put them in the place where they belong in the scene.
		- Generates the initial eye point
		- finds the location of all of the uniform variables from the vertex shader.
		- loads the aray of vertices and normals into the buffer.

- **Displaying**
	- Anytime the display function is called, several things are done:
		1. The attenuation constants are sent to the vertex shader
		2. For each of the objects, the ambient, diffuse, and specular products, and shininess are sent to the vertex shader, the current light position is sent to the vertex shader, and the transformation matrix for that object is sent to the vertex shader.
		3. The projection and model view matrices are sent to the vertex shader.

- **Movement in scene**
	- The user can move the camera in the scene by using the arrow key. This is achieved simply by adjusting xRotation and yRotation values which are used to calculate the eye position.
	- Zooming in and out is achieved by changing the radius of the eye point from the origin.
	- The light position can be adjusted by using [u]p, [d]own, [f]ront, [b]ack, [l]eft, [r]ight. This will move the light in the specified direction by one unit.
		- The light_ctm is adjusted accordingly, as well as a vector which keeps track of the current light position. This is needed in the vertex shader to ensure accurate lighting/shadows.

- **Animation**
	- Animation is achieved by keeping a flag for whether or not the scene is animating or not. Animation can be turned on using the 'a' key and off using the 'x' key. After the 'a' key is pressed, the isAnimating flag is set to 1. The idle function then rotates the ctm for the green, blue, yellow and orange balls by different amounts about the y axis, which causes the balls to "roll" around the red ball at the center.

### vshader_ctm.glsl

- All of the lighting effects are done in the vertex shader.
- The following are sent in to the vertex shader as uniform variables:
	- mat4s
		- ctm
		- model_view
		- projection
	- vec4s
		- AmbientProduct, DiffuseProduct, SpecularProduct
		- LightPosition
	- floats
		- shininess
		- attenuation values
	- int (used as boolean)
		- isShadow

- **Lighting**
	- The final color for each vertex is calculated based on the ambient, diffuse, and specular products, as well as the attenuation. The Phong lighting model is used to calculate what the color should be for each vertex. The lighting product values that are passed in are the product of the light and the material for each lighting type (ambient, specular, diffuse).
	- The lightPosition is important to determine the distance from the objects, as well as the angle from the viewer, to the object, and the light.
- **Shadows**
	- Shadows are calculated when the isShadows flag is 1.
	- Shadows are calculated by finding the proportion between the light source and the position of a vertex, and the light source and the surface of the table. The proportion of x and y is the same between the light and object as the light and the surface of the table. We calculate the x and z coordinates where a shadow belongs based on the light position and the position of a vertex. We set the y value to 0.001 so it looks like it is on the table, and we set the color to black.
