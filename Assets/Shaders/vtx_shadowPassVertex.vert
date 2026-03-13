#define U_OBJECT_MATRIX u_objectMtx
#define U_LIGHT_MATRIX u_lightMtx
#define U_SHADOWMAP u_shadowMap
#define U_PROJECTION_MATRIX u_projectionMtx



// - setup vars -----------------------------------------------------
layout (location = 0) in vec3 a_position; 

uniform mat4 U_OBJECT_MATRIX;
uniform mat4 U_LIGHT_MATRIX;

void main(){
    gl_Position = U_LIGHT_MATRIX * U_OBJECT_MATRIX * vec4(a_position, 1.0);
}