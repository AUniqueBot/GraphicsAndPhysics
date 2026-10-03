#version 460 core

// ubos shadows


// ------------------------------------------------------------------------------------
// shadow sampling.
const vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.09418410, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
);


// ------------------------------------------------------------------------------------
// shadows

float GetRandomAngle(vec2 position) {
    return fract(sin(dot(position.xy, vec2(12.9898, 78.233))) * 43758.5453) * 6.28318530718; // 2 * pi
}

float PCF_Grid(
    vec2 position, 
    vec2 texelSize, 
    int layerid,
    int halfKernelLength, 
    float currentDepth, 
    float bias,
    sampler2DArrayShadow shadowMap
    ) {
    
    float accShadowVal = 0;
    // PCF shading
    for (float u = -halfKernelLength; u <= halfKernelLength; ++u) {
        for (float v = -halfKernelLength; v <= halfKernelLength ; ++v) {
            vec2 offset = texelSize * vec2(u,v);
            float closestDepth = texture(shadowMap, vec4(position + offset, layerid, currentDepth - bias));
            accShadowVal += (currentDepth - bias > closestDepth) ? 0.0 : 1.0;
        }
    }
    float kernel = float(2 * halfKernelLength + 1);
    return accShadowVal /= (kernel * kernel);
}


float PCF_PoissonDisk(
    vec2 position,
    vec2 minBound,
    vec2 maxBound,
    vec2 texelSize,
    int layerid,
    float filterRadius,
    float currentDepth,
    float bias,
    sampler2DArrayShadow shadowMap
) {
    float accShadowVal = 0;
    float angle = GetRandomAngle(position);
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    float testedDepth = currentDepth - bias;
    for (int i = 0; i < 16; ++i) {
        vec2 rotatedDisk = poissonDisk[i];

        rotatedDisk = vec2(
            rotatedDisk.x * cosAngle - rotatedDisk.y * sinAngle,
            rotatedDisk.x * sinAngle + rotatedDisk.y * cosAngle
        );
        // to worry about - Texel Size & size of thing.
        vec2 sampledPosition = poissonDisk[i] * texelSize * filterRadius;
        sampledPosition += position;
        sampledPosition = clamp(sampledPosition, minBound, maxBound);
        float shadow = texture(shadowMap, vec4(sampledPosition, layerid, testedDepth));
        accShadowVal += shadow;
    }

    return accShadowVal / 16.0;
}

float CalculateDirectionalShadow(
    ShadowData shadowData,
    vec3 fragPosition,
    vec3 viewPosition,
    vec2 framebufferSize,
    float bias
) {
    // range of the thing. idk.

    float shadowLowest = 1.0;
    float fragDepth = abs(viewPosition.z);
    int fragDepthIndex = min(int(fragDepth / 50.0), 4);
    if (fragDepthIndex == 4) return 1.0;
    // based on index pick or something.
    vec2 tileOffset = shadowData.atlasOffsetSize[fragDepthIndex].xy;
    vec2 tileSize = shadowData.atlasOffsetSize[fragDepthIndex].zw;
    int shadowId = int(shadowData.lightTypeShadowId.y);

    // frag_pos -> light space.
    mat4 lightSpaceMatrix = shadowData.lightMatrix[fragDepthIndex];
    vec4 fragLightSpace = lightSpaceMatrix * vec4(fragPosition, 1.0);
    vec3 fragClipSpace = fragLightSpace.xyz / fragLightSpace.w;
    fragClipSpace = fragClipSpace * 0.5 + 0.5;
    
    
    // quick rejects
    if (fragClipSpace.x < 0.0 || fragClipSpace.x > 1.0) return 1.0;
    if (fragClipSpace.y < 0.0 || fragClipSpace.y > 1.0) return 1.0;
    if (fragClipSpace.z < 0.0 || fragClipSpace.z > 1.0) return 1.0;

    // sampling 
    vec2 tileMinNormalized = tileOffset / framebufferSize;
    vec2 tileSpaceNormalized = tileSize / framebufferSize;
    vec2 coords = tileMinNormalized + tileSpaceNormalized * fragClipSpace.xy;

    // bounds
    vec2 minBound = tileOffset;
    vec2 maxBound = tileOffset + tileSize;

    // reference texel size
    vec2 texelSize = 1.0 / framebufferSize;
    // texel size needs to account of the level.
    float texelScale = tileSize.x / framebufferSize.x;  // we are assuming the tile size scale is constant!
    texelSize *= texelScale;

    shadowLowest = PCF_PoissonDisk(
        coords,
        minBound,
        maxBound,
        texelSize,
        shadowId,
        4.0,
        fragClipSpace.z,
        bias,
        u_directionalShadowMap
    );

    // get distance away from camera
    // return vec3(coords, 0.0).x;
    return shadowLowest;
}

float CalculatePointShadow(
    ShadowData shadowData,
    vec3 fragPosition,
    vec3 lightPosition,
    vec2 framebufferSize,
    float bias
) {
    /*
        texture is a 6 * 1 framebuffer
        
        [][][][][][]
        
        
    */
    vec3 dirVec = fragPosition - lightPosition;
    float dist = length(dirVec);
    // get greatest magnitude.
    // [0|1], [2|3], [4|5]
    // -x, +x, -y, +y, -z, +z
    vec3 dirAbs = abs(dirVec);
    int axis =
        dirAbs.x >= dirAbs.y && dirAbs.x >= dirAbs.z ? 0 :
        dirAbs.y >= dirAbs.x && dirAbs.y >= dirAbs.z ? 1 :
        2;
    int side = axis * 2 + (dirVec[axis] > 0.0 ? 1 : 0);


    vec2 texelSize = 1.0 / framebufferSize;
    vec2 tileSize = framebufferSize.yy;   // y is never extended.
    float xoffset = (side * tileSize.x);
    int shadowId = int(shadowData.lightTypeShadowId.y);

    mat4 lightSpaceMatrix = shadowData.lightMatrix[side];
    vec4 fragLightSpace = lightSpaceMatrix * vec4(fragPosition, 1.0);
    vec3 fragClipSpace = fragLightSpace.xyz / fragLightSpace.w;
    fragClipSpace = fragClipSpace * 0.5 + 0.5;


    vec2 tileMinNormalized = vec2(xoffset, 0.0) / framebufferSize; // 0 - 1
    vec2 tileSpaceNormalized = tileSize / framebufferSize;
    vec2 coords = tileMinNormalized + tileSpaceNormalized * fragClipSpace.xy;
    vec2 minBound = tileMinNormalized;
    vec2 maxBound = vec2(xoffset + framebufferSize.y, framebufferSize.y) / framebufferSize;


    float shadowLowest = PCF_PoissonDisk(
        coords,
        minBound,
        maxBound,
        texelSize,
        shadowId,
        4.0,
        fragClipSpace.z,
        bias,
        u_pointLightShadowMap
    );

    return shadowLowest;
}


