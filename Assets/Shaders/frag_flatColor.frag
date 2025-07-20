#version 460 core


in vec3 frag_position;
uniform vec4 u_surfaceColor;
out vec4 out_color;


void main() {
	vec4 color = u_surfaceColor;
	color = vec4(1, 1, 1, 0.5);
	out_color = color;
}