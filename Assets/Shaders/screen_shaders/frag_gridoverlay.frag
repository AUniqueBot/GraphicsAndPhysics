#version 460 core

float EPSILON = 1.0e-9;

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


layout (binding=0) uniform usampler2D u_objId;
layout (binding=1) uniform sampler2D u_color;
layout (binding=2) uniform sampler2D u_depth;

layout (location = 0) out uint out_objectId;
layout (location = 1) out vec4 out_color;

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
        -1.0
    );
    
    mat3 viewToWorldRotation = transpose(mat3(COMMONPARAMS.viewMatrix));
    vec3 rayDirection = normalize(viewToWorldRotation * viewRay);

    Ray ray = Ray(
        rayDirection,
        COMMONPARAMS.cameraPosition
    );
    return ray;
}
bool intersect_rayplane(in Plane _plane, in Ray _ray, out float _t) {
    float dotDN = dot(_ray.direction, _plane.normal);
    
    if (abs(dotDN) < 0.0) {
        return false;
    }
    
    vec3 rayToPlane = _plane.origin - _ray.origin;
    float dotRN = dot(rayToPlane, _plane.normal);
    
    _t = dotRN / dotDN;
    return _t >= -EPSILON; 
}

Plane GRID = Plane(
    vec3(0.0),
    vec3(0.0, 1.0, 0.0), 
    vec3(0.0, 0.0, 1.0)
);

void main() {
    vec2 uv = VERTEXOUTPUT.frag_uv;
    float POW = 8192.0;
    // Sample background values
    vec4 baseColor = texture(u_color, uv);
    out_objectId = texture(u_objId, uv).r;
    float sceneDepth = texture(u_depth, uv).r;
    
    // Power trick: emphasizes the transition so you can see shapes
    float debugDepth = pow(sceneDepth, POW); 

    out_color = vec4(vec3(debugDepth),1); // inverse test
    out_color = baseColor;
    
    // 1. Trace Ray
    Ray ray = generateray();
    float t = -1.0;

    if (!intersect_rayplane(GRID, ray, t)) {
        return; 
    }
    vec3 intersectposition = rayatpoint(ray, t);

    vec4 clipPos = COMMONPARAMS.projectionMatrix * COMMONPARAMS.viewMatrix * vec4(intersectposition, 1.0);
    vec3 gridNdc = clipPos.xyz / clipPos.w; 
    float gridDepth = gridNdc.z * 0.5 + 0.5; // Standard OpenGL depth range mapping
    float debugGridDepth = pow(gridDepth, POW);

    if (gridDepth > sceneDepth) {
        return; 
    }

    // 3. Coordinate mapping
    vec3 u = GRID.up;
    vec3 r = cross(GRID.normal, u);
    vec3 relativeIntersection = intersectposition - GRID.origin;
    vec2 gridRelativePos = vec2(
        dot(r, relativeIntersection),
        dot(u, relativeIntersection)
    );

    // 4. Smooth Anti-Aliasing (Minor & Major)
    float gridIntervalMinor = 10.0;
    float gridIntervalMajor = gridIntervalMinor * 10.0;
    float lineMinorWidthPx = 2.5; 
    float lineMajorWidthPx = 5.5; 

    vec2 derivative = fwidth(gridRelativePos);
    vec2 halfMinorThickness = derivative * lineMinorWidthPx * 0.5;
    vec2 halfMajorThickness = derivative * lineMajorWidthPx * 0.5;

    // Symmetry fix for origin crossing
    vec2 absGridPos = abs(gridRelativePos);

    // --- MINOR LINES ---
    vec2 cellCoordMinor = mod(absGridPos, gridIntervalMinor);
    vec2 distToMinor = min(cellCoordMinor, gridIntervalMinor - cellCoordMinor);
    vec2 minorAlphaModifier = smoothstep(halfMinorThickness, -halfMinorThickness, distToMinor);
    float minorAlpha = max(minorAlphaModifier.x, minorAlphaModifier.y);

    // --- MAJOR LINES ---
    vec2 cellCoordMajor = mod(absGridPos, gridIntervalMajor);
    vec2 distToMajor = min(cellCoordMajor, gridIntervalMajor - cellCoordMajor);
    vec2 majorAlphaModifier = smoothstep(halfMajorThickness, -halfMajorThickness, distToMajor);
    float majorAlpha = max(majorAlphaModifier.x, majorAlphaModifier.y);

    // 5. Blending & Distance Falloff
    vec4 gridMinorColor = vec4(0.13, 0.13, 0.13, 1.0);
    vec4 gridMajorColor = vec4(0.7725, 0.7725, 0.7725, 1.0);

    // Combine minor and major lines
    vec4 combinedGridColor = mix(baseColor, gridMinorColor, minorAlpha);
    combinedGridColor = mix(combinedGridColor, gridMajorColor, majorAlpha);

    // --- DISTANCE FADE (Squared Falloff) ---
    // Assuming ray.origin is the camera position in world space
    float sqDist = dot(intersectposition - ray.origin, intersectposition - ray.origin);
    
    // Tweak these values to change where the fade starts and ends
    float fadeStartSq = 50.0 * 50.0;   // Starts fading at 50 units
    float fadeEndSq = 500.0 * 500.0;   // Completely gone at 500 units
    
    // Calculate fading factor (1.0 = fully visible, 0.0 = completely faded out)
    float fadeFactor = 1.0 - smoothstep(fadeStartSq, fadeEndSq, sqDist);

    // Final application of the grid over background based on distance fade
    out_color = mix(baseColor, combinedGridColor, fadeFactor);
}

