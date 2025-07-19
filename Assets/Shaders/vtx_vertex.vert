#version 460 core


#define U_OBJECT_MATRIX u_objectMtx
#define U_CAMERA_MATRIX u_cameraMtx
#define U_PROJECTION_MATRIX u_projectionMtx



// - setup vars -----------------------------------------------------
layout (location=0) in vec3 a_position; 
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 U_OBJECT_MATRIX;
uniform mat4 U_CAMERA_MATRIX;
uniform mat4 U_PROJECTION_MATRIX;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_uv;

void main(){
    // do something here
    gl_Position = U_PROJECTION_MATRIX * U_CAMERA_MATRIX * U_OBJECT_MATRIX * vec4(a_position, 1.0);

    frag_position = vec3(gl_Position);
    frag_normal = a_normal;
    frag_uv = a_uv;
    
}