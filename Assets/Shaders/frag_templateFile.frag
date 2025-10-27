#version 460 core

/*
    here's a sample file for a shader.
    naming conventions are important in this scenario so this file should be a good jumping off point.

    ensure the filenames are also standardised.

*/

/*
    naming conventions for attrs.
    | **Stage**       | **Variable Prefix** | Example     |
    | --------------- | ------------------- | ----------- |
    | Vertex Input    | `a` (attribute)     | `aPosition` |
    | Vertex Output   | `v` (varying)       | `vNormal`   |
    | Fragment Input  | `v` (from vertex)   | `vNormal`   |
    | Fragment Output | `frag` or `out`     | `fragColor` |


    aPos - vertex input, attribute, position.
    vRot - vertex output, attribute, rotation.


*/



// - define standardised names here ---------------------------------
#define U_OBJECT_MATRIX u_objectMtx
#define U_CAMERA_MATRIX u_cameraMtx
#define U_PROJECITON_MATRIX u_projectionMtx



// - setup vars -----------------------------------------------------
in vec3 vPos; 
out vec4 outCol;

uniform mat4 U_OBJECT_MATRIX;
uniform mat4 U_CAMERA_MATRIX;
uniform mat4 U_PROJECTION_MATRIX;



void main(){
    // do something here
    outCol = vec4(1.0, 0.0, 1.0, 1.0);
}