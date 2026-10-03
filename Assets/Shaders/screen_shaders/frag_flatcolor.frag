#version 460 core

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


void main() {
    gl_FragColor = vec4(1.0, 0.0, 0.75, 1.0);
}