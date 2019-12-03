#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;
varying vec4 color;

uniform mat4 ctm;
uniform mat4 cubie_ctm;
vec4 lightPosition = vec4(0, 0, 5, 1);
float shininess = 10;
vec4 ambient = vec4(.2, .2, .2, 1);
vec4 diffuse = vec4(1, 1, 1, 1);
vec4 specular = vec4(1, 1, 1, 1);
float attConst = 1;
float attLinear = 0;
float attQuad = 0;
vec4 a, d, s;



void main()
{

	vec4 n = normalize(ctm * cubie_ctm * vNormal);
	vec4 lTemp = lightPosition - (ctm * cubie_ctm * vPosition);
	vec4 L = normalize(lTemp);
	vec4 d = max(dot(L, n), 0) * diffuse;
	vec4 eye = vec4(0, 0, 1, 1);
	eye = ctm * eye;
	vec4 V = normalize(eye - (ctm * cubie_ctm * vPosition));
	vec4 H = normalize(L + V);
	vec4 s = pow(max(dot(n, H), 0.0), shininess) * specular;
	float distance = length(lTemp);
	float attenuation = 1/(attConst + (attLinear * distance) + (attQuad * distance * distance));
	vec4 temp = vColor * (ambient + (attenuation * (d+s)));
	color = temp;
	gl_Position = ctm * cubie_ctm * vPosition;
}
