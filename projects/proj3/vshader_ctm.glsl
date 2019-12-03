#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;
//attribute vec4 vColor;
varying vec4 color;

uniform mat4 ctm;
uniform mat4 model_view;
uniform mat4 projection;
uniform int isShadow;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float shininess;
uniform float attenuation_constant, attenuation_linear, attenuation_quadratic;
vec4 ambient, diffuse, specular;

void main()
{
	ambient = AmbientProduct;
	vec4 N = normalize(projection * model_view * ctm * vNormal);
	vec4 L_temp = projection * model_view * (LightPosition - (ctm * vPosition));
	vec4 L = normalize(L_temp);
	diffuse = max(dot(L,N), 0.0) * DiffuseProduct;
	vec4 eyePosition = vec4(0, 0, 0, 1);
	vec4 V = normalize(eyePosition - (projection * model_view * vPosition));
	vec4 H = normalize(L + V);
	specular = pow(max(dot(N, H), 0.0), shininess) * SpecularProduct;
	float distance = length(L_temp);
	float attenuation = 1 /(attenuation_constant + (attenuation_linear * distance) + (attenuation_quadratic * distance * distance));
	
	color = 2*ambient + (attenuation * (diffuse + specular));
	// color = vNormal;
	gl_Position = projection * model_view * ctm * vPosition;
}
