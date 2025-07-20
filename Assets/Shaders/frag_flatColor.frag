#version 460 core


in vec3 frag_position;
uniform vec4 u_surfaceColor;
out vec4 out_color;


void main() {
	out_color = vec4(1, 1, 1, 0.5);
}