#version 460 core

float EPSILON = 1.0e-9;

// from draw
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

// don't worry about 

layout (binding=0) uniform sampler2D u_color;
layout (binding=1) uniform sampler2D u_depth;


layout (location = 0) out vec4 out_color;


struct Ray {
    vec3 direction;
    vec3 origin;
};

struct Plane {
    vec3 origin;
    vec3 normal;
    vec3 up;
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
        1.0
    );
    vec3 rayDirection = normalize(
        mat3(COMMONPARAMS.cameraMatrix) * viewRay
    );

    Ray ray = Ray(
        rayDirection,
        VERTEXOUTPUT.frag_viewPosition
    );
    return ray;
}





bool intersect_rayplane(in Plane _plane, in Ray _ray, inout float _t) {
    vec3 rayToPlane = _plane.origin - _ray.origin;
    float dotDN = dot(_ray.direction, _plane.normal);
    float dotRN = dot(rayToPlane, _plane.normal);
    
    if (abs(dotDN) <= EPSILON) {
        return false;
    }
    _t =  dotRN / dotDN;
    return _t >= EPSILON; 
}

Plane GRID = Plane(
    vec3(0),
    vec3(0, 1, 0),
    vec3(0, 0, 1)
);



void main() {


    out_color = texture(u_color, VERTEXOUTPUT.frag_uv);
    float depth = texture(u_depth, VERTEXOUTPUT.frag_uv).r;

    Ray ray = generateray();
    float t = -1.0;

    if (!intersect_rayplane(GRID, ray, t)) {
        return;
    }
    
    return;
    vec3 intersectposition = rayatpoint(ray, t);

    vec3 u = GRID.up;
    vec3 r = cross(GRID.normal, u);
    vec3 rayToPlane = GRID.origin - ray.origin;
    vec2 gridRelativePos = vec2(
        dot(r, rayToPlane),
        dot(u, rayToPlane)
    );

    float gridInterval = 1.0; 
    vec2 distFromNearest = mod(gridRelativePos, gridInterval);
    vec2 gridNearestInterval = gridRelativePos - distFromNearest;
    vec4 gridColor = vec4(0.5);
    // depth test with value. idk.
    // depth is a value of 0 - 1 based on near/far clip planes.

    vec4 clipPos = 
        COMMONPARAMS.projectionMatrix * 
        COMMONPARAMS.cameraMatrix * 
        vec4(intersectposition, 1.0);
    vec3 ndc = clipPos.xyz / clipPos.w * 0.5 + 0.5; 

    if (ndc.z >= depth) {
        return;
    }

    if (distFromNearest.x <= EPSILON || distFromNearest.y <= EPSILON) {
        out_color = gridColor;
    }
}