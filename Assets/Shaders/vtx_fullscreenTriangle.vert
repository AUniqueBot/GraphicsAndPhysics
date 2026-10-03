#version 460 core // hide on thing idk


out VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
	vec3 frag_viewPosition;
} VERTEXOUTPUT;

const vec2 vertices[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);
void main() {
    vec2 pos = vertices[gl_VertexID];
    VERTEXOUTPUT.frag_uv = pos * 0.5 + 0.5;   // convert clip space UV
    gl_Position = vec4(pos, 0.0, 1.0);
}