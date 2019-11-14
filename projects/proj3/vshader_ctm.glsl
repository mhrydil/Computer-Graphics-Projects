#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTexCoord;

varying vec4 color;
varying vec2 texCoord;

uniform mat4 ctm;
uniform mat4 model_view;
uniform mat4 projection;

void main()
{
	texCoord = vTexCoord;
	gl_Position = projection * model_view * ctm * (vPosition / vPosition.w);
	color = vColor;
}
