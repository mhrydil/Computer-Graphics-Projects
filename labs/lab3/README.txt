Matthew Hrydil
mbh38
CS1566 Lab 3

Submitted September 27, 2019

Included in submission:


'make cone' compiles cone.c
Run the file by running the command ./cone

**I use a mac, for what it's worth.

cone.c creates a 3d cone by calling two functions: fillEdges() and fillColors. FillEdges generates the location for each of the vertices of cone. It adds the three vertices for each triangle in order to the vertices[] array. It can create a cone with an arbitrary number of vertices using the numVertices parameter. If this is done, the length of the vertices and colors global arrays must be adjusted.

The fillColors function generates random colors for each set of three vertices, ensuring that each triangle that composes the cone is made up of a random color.


