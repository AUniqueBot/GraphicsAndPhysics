#define U_OBJECT_MATRIX u_objectMtx
#define U_CAMERA_MATRIX u_cameraMtx
#define U_PROJECTION_MATRIX u_projectionMtx
#define U_DELTA_TIME u_deltaTime;



// - setup vars -----------------------------------------------------
layout (location = 0) in vec3 a_position; 
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 U_OBJECT_MATRIX;
uniform mat4 U_CAMERA_MATRIX;
uniform mat4 U_PROJECTION_MATRIX;
uniform float U_DELTA_TIME;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_uv;

void main(){
    vec4 worldPos = U_OBJECT_MATRIX * vec4(a_position, 1.0);
    gl_Position = U_PROJECTION_MATRIX * U_CAMERA_MATRIX * worldPos;

    mat3 normalMatrix = transpose(inverse(mat3(U_OBJECT_MATRIX)));
    frag_normal = normalize(normalMatrix * a_normal);

    frag_position = worldPos.xyz; // world-space position
    frag_uv = a_uv;
}