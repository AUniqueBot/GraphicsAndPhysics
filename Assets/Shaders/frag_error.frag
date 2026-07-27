#version 460 core


in VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
	vec3 frag_viewPosition;
} VERTEXOUTPUT;


layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;
layout (location = 2) out vec4 out_litShadow;

layout (std140, binding=1) uniform ObjectUBO {
	mat4 objectMatrix;
	vec3 position;
	uint objectId;
} OBJECTPARAMS;


void main() {
    out_objectId = OBJECTPARAMS.objectId;



    out_color = vec4(1.0, 0.0, 1.0, 1.0);
    // checkerboard pattern based on uv.
    // colors - magenta and black.

    


    return;
}