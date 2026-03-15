// #version 440 core // hide this on compile



#define LIGHT_POINT 0.0
#define LIGHT_SPOT 1.0
#define LIGHT_DIRECTIONAL 2.0
#define LIGHT_AMBIENT 3.0

#define MAX_LIGHT_COUNT 10
#define MAX_SHADOW_COUNT 10
#define SHADOW_MAP_MATRIX_COUNT 6

// ------------------------------------------------------------------------------------
// input

in VertexOutput {
    vec3 frag_position;
    vec3 frag_normal;
    vec2 frag_uv;
} VERTEXOUTPUT;

uniform sampler2D u_albedo;
uniform sampler2DArray u_directionalShadowMap;
uniform sampler2DArray u_spotLightShadowMap;
uniform samplerCubeArray u_pointLightShadowMap;


// ------------------------------------------------------------------------------------
// output
layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;


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
} ;
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

float PercentageCloserFilter(
    vec2 position, 
    vec2 texelSize, 
    int layerid,
    int halfKernelLength, 
    float currentDepth, 
    float bias,
    sampler2DArray shadowMap
    ) {

    float accShadowVal = 0;
    // PCF shading
    for (float u = -halfKernelLength; u <= halfKernelLength; ++u) {
        for (float v = -halfKernelLength; v <= halfKernelLength ; ++v) {
            vec2 offset = texelSize * vec2(u,v);
            float closestDepth = texture(shadowMap, vec3(position + offset, layerid)).r;
            accShadowVal += (currentDepth - bias > closestDepth) ? 0.0 : 1.0;
        }
    }
    float kernel = float(2 * halfKernelLength + 1);
    return accShadowVal /= (kernel * kernel);
}

float CalculateDirectionalShadow(
    ShadowData shadowData,
    vec3 fragPosition,
    vec2 texelSize, 
    vec2 framebufferSize,
    float bias
) {
    float shadowLowest = 1.0;
    // vec2 tileOffset = shadowData.m_atlasOffsetSize.xy;
    // vec2 tileSize = shadowData.m_atlasOffsetSize.zw;
    // int shadowId = int(shadowData.m_lightTypeShadowId.y);
    // get distance away from camera
    return shadowLowest;
}



float CalculateShadow() {
    float shadowLowest = 0.0;
    for (int i= 0; i < SHADOWPARAMS.directionalShadowCount; ++i) {
        ShadowData currentShadowData = SHADOWPARAMS.shadowData[i]; 
        int lightType = int(currentShadowData.lightTypeShadowId.x);
        if (LIGHT_DIRECTIONAL == lightType) {
            shadowLowest = -1.0;
            // sample the texture.
        }
        else if (LIGHT_POINT == lightType) {

        }
        else if (LIGHT_SPOT == lightType) {

        }

    }

    return 1.0 + shadowLowest;

}


// ------------------------------------------------------------------------------------
// lighting helper functions

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



vec3 CalculateLighting(vec3 fragPosition, vec3 fragNormal) {
    vec3 N = normalize(fragNormal);
    vec3 result = vec3(0.0);
    vec3 ambientRes = vec3(0.0);
    for (int i = 0; i < LIGHTPARAMS.m_lightCount; ++i) {
        LightData currentLight = LIGHTPARAMS.m_lightData[i];
        int lightType = int(currentLight.position_type.w);
        // point light
        if (lightType == LIGHT_POINT) {
            result += CalculatePointLighting(
                currentLight,
                 fragPosition,
                  N
                );
        }


        // directional
        else if (lightType == LIGHT_DIRECTIONAL) {
            result += CalculateDirectionalLighting(
                currentLight,
                 N
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
    // result *= 1;
    return result * CalculateShadow() + ambientRes;
    // return result + ambientRes;
}

// ------------------------------------------------------------------------------------


void main() {
	vec4 color = texture(u_albedo, VERTEXOUTPUT.frag_uv);
    out_objectId = OBJECTPARAMS.objectId;
    // out_color = float(u_objectId % 256u) / 255.0; // testing
    out_color = color * vec4(
        CalculateLighting(
            VERTEXOUTPUT.frag_position,
             VERTEXOUTPUT.frag_normal
             ), 
             1.0 // no alpha needed
    );
}


