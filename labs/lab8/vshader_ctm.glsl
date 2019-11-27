#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 ctm;
uniform mat4 model_view_matrix;
uniform mat4 projection;
uniform vec4 light_position;
uniform int is_shadow;

void main()
{
	// gl_Position = ctm * vPosition;
	// color = vColor;
	if(is_shadow == 0){
		gl_Position = projection * model_view_matrix * vPosition;
		color = vColor;
	}
	else{
		float x = 0;
		float z = 0;
		gl_Position = model_view_matrix * vec4(x, 0, z, 1);
		color = vec4(0, 0, 0, 1);
	}
}
