#version 460 core


// - define standardised names here ---------------------------------
#define U_OBJECT_MATRIX u_objectMtx
#define U_CAMERA_MATRIX u_cameraMtx
#define U_PROJECITON_MATRIX u_projectionMtx

// for now, start with the basics and only focus on position.
// this describes how the vertex data is like -> [VEC3 : Pos] [VEC2 : UV]
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNml;
layout (location = 2) in vec2 aUv;

out vec3 fragPos;	// out position for every pixel
out vec2 oUv;		// output to fragment shader
out vec3 oNml;

// - camera -----------------------------------------------------------------------
uniform mat4 U_OBJECT_MATRIX;			// object transform matrix
uniform mat4 U_CAMERA_MATRIX;			// camera's view matrix (pos + rot, inverse)
uniform mat4 U_PROJECITON_MATRIX;	// camera's view frustum

void main() {
	
	gl_Position = U_PROJECITON_MATRIX * U_CAMERA_MATRIX * U_OBJECT_MATRIX * vec4(aPos, 1.0f);
	fragPos = vec3(U_OBJECT_MATRIX * vec4(aPos, 1.0));				

	// Set the color of the vertex
	oUv = aUv; // pass the texture coordinates to the fragment shader
	oNml = mat3(transpose(inverse(mat3(U_OBJECT_MATRIX)))) * aNml; // normal mtx, no translate.
}
