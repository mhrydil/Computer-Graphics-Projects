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
	if(isShadow == 0) // Draw the actual objects
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
		color = ambient + (attenuation * (diffuse + specular));
		gl_Position = projection * model_view * ctm * vPosition;
	}
	else //draw the fake shadows
	{
		vec4 light = LightPosition;
		vec4 ballVector = ctm * vPosition;
		float newX = light.x - (light.y * ((light.x - ballVector.x) / (light.y - ballVector.y))); //light x pos, ball x 0
		float newZ = light.z - (light.y * ((light.z - ballVector.z) / (light.y - ballVector.y)));
		vec4 newVector = vec4(newX, 0.001, newZ, 1);
		gl_Position = projection * model_view * newVector;
		color = vec4(0, 0, 0, 1);
	}
}
