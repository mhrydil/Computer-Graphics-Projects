# CS1566
## October 24, 2019

- send array of 108 vertices to graphics pipeline
	- first and second sets of 36 are for top left and top right cubes
	- use 36 vertices, scale and move to position and then send to graphics pipeline
	- third set of 36 vertices is the original cube without any modification (1x1x1)

***

## Lighting

- We need to model
	- light sources
	- light-material interactions
- Lighting model can be applied in various parts:
	- application,
	- vertex shader, or
	- fragment shader

- LIght-Material Interaction
	- Specular surfaces: Shiny - reflect light in a narrow angle
	- Diffuse surfaces: matte - reflect light equally in all directions
	- Translucent surfaces: transparent - let light pass through

- Light Sources
	- a location that emits light
	- Every point (x, y, z) on the surface of a light source has its own characteristic.
		- intensity of energy emitted at each wave length (lambda)

- Phong Reflection Method
	- A light source has ambient, diffuse, and specular terms
	- Each term contains three colors: red, green, and blue
	- For any point **p** on a surface we need a 3x3 illumination matrix for each light source *i*:
		- [L~*i*ra~	L~*i*ga~	L~*i*ba~]
		- [L~*i*rd~	L~*i*gd~	L~*i*bd~]
		- [L~*i*rs~	L~*i*gs~	L~*i*bs~]