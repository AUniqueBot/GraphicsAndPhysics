#version 460 core

// for now, start with the basics and only focus on position.
// this describes how the vertex data is like -> [VEC3 : Pos] [VEC3 : Col] [VEC2 : UV]
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aUv;

out vec4 vtxPos;
out vec3 aColour;
out vec2 oUv; // output to fragment shader

uniform float x_offset;
uniform float y_offset;

uniform vec4 camera; // ignore for now

uniform mat4 trs;

void main() {
	// Set the position of the vertex
	gl_Position = trs * vec4(aPos, 1.0f);
	// Set the color of the vertex
	
	aColour = aCol;
	oUv = aUv; // pass the texture coordinates to the fragment shader
}
