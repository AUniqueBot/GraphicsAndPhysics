#version 460 core
in VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
	vec3 frag_viewPosition;
} VERTEXOUTPUT;



layout (std140, binding=0) uniform CommonUBO {
	mat4 cameraMatrix;			// 16
	mat4 projectionMatrix;		// 32
	vec3 cameraPosition;			
	int _pad1;
	vec3 cameraForward;
	int _pad2;
	vec3 cameraUp;
	float deltaTime;
} COMMONPARAMS;

layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;
layout (location = 2) out vec4 out_litShadow;

void main() {
    
    vec3 rayDirection = normalize(
        COMMONPARAMS.cameraForward
    );

    float t = rayDirection.y * 0.5 + 0.5;

    out_color = mix(
        vec4(0.1922, 0.1922, 0.1922, 1.0),
        vec4(0.5765, 0.7451, 1.0, 1.0),
        t
    );
}