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
	float aspectRatio;
	vec3 cameraForward;
	float cameraVFov;
	vec3 cameraUp;
	float deltaTime;
} COMMONPARAMS;

layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;
layout (location = 2) out vec4 out_litShadow;

void main() {
    vec2 ndc = VERTEXOUTPUT.frag_uv * 2.0 - 1.0;

    float halfHeight = tan(radians(COMMONPARAMS.cameraVFov) * 0.5);
    float halfWidth = halfHeight * COMMONPARAMS.aspectRatio;

    vec3 viewRay = vec3(
        ndc.x * halfWidth,
        ndc.y * halfHeight,
        -1.0
    );

    vec3 rayDirection = normalize(
        mat3(COMMONPARAMS.cameraMatrix) * viewRay
    );

    float gradientFactor = rayDirection.y * 0.5 + 0.5;

    vec3 finalColor = mix(
        vec3(0.2549, 0.251, 0.251),
        vec3(0.6627, 0.8196, 1.0),
        gradientFactor
    );

    
    out_color = vec4(finalColor, 1.0);
    out_objectId = 0U; // Clean output for standard pass requirements
    out_litShadow = vec4(0.0);
}