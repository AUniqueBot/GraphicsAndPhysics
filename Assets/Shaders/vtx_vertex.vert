// #version 440 core

// - setup vars -----------------------------------------------------
layout (location = 0) in vec3 a_position; 
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

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

layout (std140, binding=1) uniform ObjectUBO {
	mat4 objectMatrix;
	vec3 position;
	uint objectId;
} OBJECTPARAMS;

out VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
} VERTEXOUTPUT;


void main(){
    vec4 worldPos = OBJECTPARAMS.objectMatrix * vec4(a_position, 1.0);
    gl_Position = COMMONPARAMS.projectionMatrix * COMMONPARAMS.cameraMatrix * worldPos;

    mat3 normalMatrix = transpose(inverse(mat3(OBJECTPARAMS.objectMatrix)));

    VERTEXOUTPUT.frag_position = worldPos.xyz;
    VERTEXOUTPUT.frag_normal = normalize(normalMatrix * a_normal);
    VERTEXOUTPUT.frag_uv = a_uv;
}