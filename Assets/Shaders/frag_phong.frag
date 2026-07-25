#version 460 core // hide this on compile


#define LIGHT_POINT 0.0
#define LIGHT_SPOT 1.0
#define LIGHT_DIRECTIONAL 2.0
#define LIGHT_AMBIENT 3.0

#define MAX_LIGHT_COUNT 10
#define MAX_SHADOW_COUNT 10
#define SHADOW_MAP_MATRIX_COUNT 6



in VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
	vec3 frag_viewPosition;
} VERTEXOUTPUT;

uniform sampler2D u_albedo;
uniform sampler2D u_specular;
uniform sampler2D u_gloss;

uniform sampler2DArrayShadow u_directionalShadowMap;
uniform sampler2DArray u_spotLightShadowMap;
uniform samplerCubeArray u_pointLightShadowMap;
uniform float u_deltaTime;
uniform uint u_objectId;

    
layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;
layout (location = 2) out vec4 out_litShadow;


// ------------------------------------------------------------------------------------
// ubos

layout (std140, binding=1) uniform ObjectUBO {
	mat4 objectMatrix;
	vec3 position;
	uint objectId;
} OBJECTPARAMS;



// ------------------------------------------------------------------------------------
// ubos light
struct LightData {
	vec4 position_type;	// 1, 2, 3, 4
	vec4 direction_roll;
	vec4 color_power;
	vec4 attenuation;
};
layout (std140, binding=2) uniform LightUBO {
	LightData m_lightData[MAX_LIGHT_COUNT];
	int m_lightCount;
} LIGHTPARAMS;

// ------------------------------------------------------------------------------------
// ubos shadows
struct ShadowData {
    mat4 lightMatrix[SHADOW_MAP_MATRIX_COUNT];
    vec4 atlasOffsetSize[SHADOW_MAP_MATRIX_COUNT];
    vec4 lightTypeShadowId;
};
layout (std140, binding=3) uniform ShadowMapUBO {
    ShadowData shadowData[MAX_SHADOW_COUNT];
	vec4 directionalAtlasResAndTexelSize;
	vec4 spotAtlasResAndTexelSize;
	vec4 pointAtlasResAndTexelSize;
    int directionalShadowCount;
    int pointShadowCount;
    int spotShadowCount;
} SHADOWPARAMS;


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
    
    
    for (int i = 0; i < 16; ++i) {
        vec2 rotatedDisk = poissonDisk[i];

        rotatedDisk = vec2(
            rotatedDisk.x * cosAngle - rotatedDisk.y * sinAngle,
            rotatedDisk.x * sinAngle + rotatedDisk.y * cosAngle
        );

        vec2 sampledPosition = poissonDisk[i] * texelSize * filterRadius;
        float closestDepth = texture(shadowMap, vec4(sampledPosition + position, layerid, currentDepth - bias));
        accShadowVal += (currentDepth - bias > closestDepth) ? 0.0 : 1.0;
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

    // poisson disk sampling.
    vec2 texelSize = 1.0 / framebufferSize;

    shadowLowest = PCF_PoissonDisk(
        coords,
        texelSize,
        shadowId,
        4,
        fragClipSpace.z,
        bias,
        u_directionalShadowMap
    );

    // get distance away from camera
    // return vec3(coords, 0.0).x;
    return shadowLowest;
}



float CalculateShadow() {
    float shadowLowest = 1.0;
    for (int i= 0; i < SHADOWPARAMS.directionalShadowCount; ++i) {
        ShadowData currentShadowData = SHADOWPARAMS.shadowData[i]; 
        int lightType = int(currentShadowData.lightTypeShadowId.x);
        if (LIGHT_DIRECTIONAL == lightType) {
            shadowLowest = min(
                CalculateDirectionalShadow(
                currentShadowData, 
                VERTEXOUTPUT.frag_position, 
                VERTEXOUTPUT.frag_viewPosition, 
                SHADOWPARAMS.directionalAtlasResAndTexelSize.xy, 
                0.01
                ), shadowLowest
                );
            // sample the texture.
        }
        else if (LIGHT_POINT == lightType) {

        }
        else if (LIGHT_SPOT == lightType) {

        }

    }

    return shadowLowest;

}


// ------------------------------------------------------------------------------------
// lighting helper functions

float CalculateSpecularHighlight(
    vec3 _lightDir, 
    vec3 _viewPosition, 
    vec3 _fragPosition, 
    vec3 _fragNormal,
    int _shininess, 
    float _specularStrength
){
    vec3 N = normalize(_fragNormal);
    vec3 incidentLightDir = normalize(_lightDir); 
    vec3 reflectDir = reflect(incidentLightDir, N);  
    vec3 viewDir = normalize(_viewPosition - _fragPosition); 
    float specDot = max(dot(viewDir, reflectDir), 0.0);

    // Compute the final generic Phong power formula
    return pow(specDot, float(_shininess)) * _specularStrength;
}


vec3 CalculateDirectionalLighting(LightData _currentLight, vec3 _fragNormal){
    // Direction is assumed to be the light direction (e.g., from which light comes)
    vec3 lightDir = normalize(_currentLight.direction_roll.xyz);
    float NdotL = max(dot(_fragNormal, -lightDir), 0.0);

    vec3 lightColor = _currentLight.color_power.xyz;
    float power = _currentLight.color_power.w;
    return lightColor * power *  NdotL;
}


vec3 CalculatePointLighting(LightData _currentLight, vec3 _fragPosition, vec3 _fragNormal) {
    vec3 lightPosition = _currentLight.position_type.xyz;
    vec3 lightVec = _fragPosition - lightPosition;
    vec3 nLightVec = normalize(lightVec);
    // we need distance and attenuation values as well to make this work.
    float dist = length(lightVec);
    float power = _currentLight.color_power.w / (dist * dist); // quadratic loss.
    // assumes distance falls off immediately for now.

    float NdotL = max(dot(_fragNormal, -nLightVec), 0.0);
    vec3 lightColor = _currentLight.color_power.xyz;
    return lightColor * power * NdotL;
}




struct LightingResult {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

LightingResult CalculateLighting(
    vec3 fragPosition, 
    vec3 fragNormal, 
    vec3 cameraPosition, 
    int glossiness,
    float strength
    ) {
    vec3 N = normalize(fragNormal);

    vec3 diffuseRes = vec3(0.0);
    vec3 specularRes = vec3(0.0);
    vec3 ambientRes = vec3(0.0);
    
    
    for (int i = 0; i < LIGHTPARAMS.m_lightCount; ++i) {
        LightData currentLight = LIGHTPARAMS.m_lightData[i];
        int lightType = int(currentLight.position_type.w);
        // point light
        if (lightType == LIGHT_POINT) {
            diffuseRes += CalculatePointLighting(
    currentLight,
    fragPosition,
    N
                );

            vec3 lightDir = normalize(fragPosition - currentLight.position_type.xyz);
            vec3 lightColor = currentLight.color_power.rgb;
            specularRes +=  lightColor * CalculateSpecularHighlight(
                lightDir, 
                cameraPosition, 
                fragPosition, 
                N,
                glossiness, strength
            );

        }


        // directional
        else if (lightType == LIGHT_DIRECTIONAL) {
            // process in 2 steps; 
            diffuseRes += CalculateDirectionalLighting(
                currentLight, 
                N
                );

            vec3 lightDir = normalize(currentLight.direction_roll.xyz);
            vec3 lightColor = currentLight.color_power.rgb;
            specularRes += lightColor * CalculateSpecularHighlight(
                lightDir, 
                cameraPosition, 
                fragPosition, 
                N,
                glossiness, strength
            );

        }

        // ambient
        else if (lightType == LIGHT_AMBIENT) {
            vec3 lightColor = currentLight.color_power.xyz;
            float power = currentLight.color_power.w;
            ambientRes += vec3(lightColor * power);
        }
    }
    ambientRes = clamp(ambientRes, 0.0, 1.0);

    return LightingResult(
        diffuseRes,
        specularRes,
        ambientRes
    );

}

// ------------------------------------------------------------------------------------


void main() {
	    
    out_objectId = OBJECTPARAMS.objectId;
    vec4 diff = texture(u_albedo, VERTEXOUTPUT.frag_uv);
    vec4 spec = texture(u_specular, VERTEXOUTPUT.frag_uv);
    vec4 gloss = texture(u_gloss, VERTEXOUTPUT.frag_uv);

    LightingResult lighting = CalculateLighting(
        VERTEXOUTPUT.frag_position, 
        VERTEXOUTPUT.frag_normal,
        VERTEXOUTPUT.frag_viewPosition,
        int(gloss.r * 16.0), 1.0
    );
    float sValue = CalculateShadow();

    vec3 ambientComponent  = lighting.ambient * diff.rgb;
    vec3 diffuseComponent  = lighting.diffuse * diff.rgb * sValue;
    vec3 specularComponent = lighting.specular * spec.rgb * sValue; // Masked by spec texture


    // Combine everything for the final frag color
    out_color += vec4(ambientComponent, 1.0); 
    out_color += vec4(diffuseComponent, 1.0);
    out_color += vec4(specularComponent, 1.0);

    // CSM testing.
    // vec4 color_RED = vec4(1.0, 0.0, 0.0, 1.0);
    // vec4 color_GREEN = vec4(0.0, 1.0, 0.0, 1.0);
    // vec4 color_BLUE = vec4(0.0, 0.0, 1.0, 1.0);
    // float depth = -VERTEXOUTPUT.frag_viewPosition.z;
    // int depthIndex = min(int(depth/50.0), 3);
    // vec4 shadowCol = depthIndex == 0 ?  color_RED : depthIndex == 1 ? color_GREEN : depthIndex == 2 ? color_BLUE: vec4(0.0, 0.0, 0.0, 1.0);
    // if (sValue != 1.0) {
    //     out_color = mix(out_color, shadowCol, 0.5);;
    // }
    
    // out_color = vec4(vec3(s), 1.0);
  
    return;
}


