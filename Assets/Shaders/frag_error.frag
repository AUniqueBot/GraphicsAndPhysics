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

float scale = 20.0;

void main() {
    out_objectId = OBJECTPARAMS.objectId;

    vec4 checkerColor1 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 checkerColor2 = vec4(1.0, 0.0, 1.0, 1.0);
    vec2 currentUv = VERTEXOUTPUT.frag_uv;

    // uv range - [0, 1] normalized.
    // normalized ranges.
    // ranges [0, 0.5] x, [0, 0.5] y - magenta
    // ranges [0.5, 1] x, [0, 0.5] y - black
    // ranges [0, 0.5] x, [0.5, 1] y - black
    // ranges [0.5, 1] x, [0.5, 1] y - magenta

    vec2 scaledUV = currentUv * float(scale);
    out_color = mod(floor(scaledUV.x) + floor(scaledUV.y), 2.0) < 1.0 ? checkerColor1 : checkerColor2;
    return;
}