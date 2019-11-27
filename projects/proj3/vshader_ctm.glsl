#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 ctm;
uniform mat4 model_view;
uniform mat4 projection;
uniform int isShadow;
uniform vec4 ambient_product, diffuse_product, specular_product;
uniform vec4 LightPosition;
uniform float shininess;
uniform float attenuation_constant, attenuation_linear, attenuation_quadratic;


void main()
{
	color = vColor;
	gl_Position = projection * model_view * ctm * (vPosition/vPosition.w);
}
