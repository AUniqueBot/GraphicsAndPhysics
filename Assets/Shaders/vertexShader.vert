#version 460 core

// for now, start with the basics and only focus on position.
// this describes how the vertex data is like -> [VEC3 : Pos] [VEC2 : UV]
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNml;
layout (location = 2) in vec2 aUv;

out vec3 fragPos;	// out position for every pixel
out vec2 oUv;		// output to fragment shader
out vec3 oNml;

uniform float x_offset;
uniform float y_offset;

uniform vec4 camera; // ignore for now

uniform mat4 trs;			// object transform matrix
uniform mat4 view;			// camera's view matrix (pos + rot, inverse)
uniform mat4 projection;	// camera's view frustum

void main() {
	
	gl_Position = projection * view * trs * vec4(aPos, 1.0f);
	fragPos = vec3(trs * vec4(aPos, 1.0));

	// Set the color of the vertex
	oUv = aUv; // pass the texture coordinates to the fragment shader
	oNml = mat3(transpose(inverse(mat3(trs)))) * aNml; // normal mtx, no translate.
}
