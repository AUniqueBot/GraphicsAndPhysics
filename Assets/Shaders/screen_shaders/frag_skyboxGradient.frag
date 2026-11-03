#version 460 core

// from draw
in VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
	vec3 frag_viewPosition;
} VERTEXOUTPUT;



layout (std140, binding=0) uniform CommonUBO {
	mat4 viewMatrix;			// 16  inversed.
	mat4 projectionMatrix;		// 32
	vec3 cameraPosition;			
	float aspectRatio;
	vec3 cameraForward;
	float cameraVFov;
	vec3 cameraUp;
    // float nearClip;
    // float farClip;
	float deltaTime;
} COMMONPARAMS;


// skybox gradient UBO

layout (std140, binding=1) uniform SkyboxUBO {
    vec3 sunDirection;
} SKYBOXPARAMS;

layout (location = 0) out vec4 out_color;


struct Ray {
    vec3 direction;
    vec3 origin;
};

vec3 rayatpoint(in Ray _ray, in float _t) {
    return _ray.origin + _ray.direction * _t;
}


Ray generateray() {
    vec2 ndc = VERTEXOUTPUT.frag_uv * 2.0 - 1.0;
    float halfHeight = tan(radians(COMMONPARAMS.cameraVFov) * 0.5);
    float halfWidth = halfHeight * COMMONPARAMS.aspectRatio;
    vec3 viewRay = vec3(
        ndc.x * halfWidth,
        ndc.y * halfHeight,
        -1.0
    );
    SKYBOXPARAMS;
    mat3 viewToWorldRotation = transpose(mat3(COMMONPARAMS.viewMatrix));
    vec3 rayDirection = normalize(viewToWorldRotation * viewRay);

    Ray ray = Ray(
        rayDirection,
        COMMONPARAMS.cameraPosition
    );


    return ray;
}



void main() {


    // 
    Ray ray = generateray();
    float gradientFactor = ray.direction.y * 0.5 + 0.5;

    vec3 finalColor = mix(
        vec3(0.0549, 0.0549, 0.0549),
        vec3(0.4706, 0.7176, 1.0),
        gradientFactor
    );

    
    out_color = vec4(finalColor, 1.0);
}