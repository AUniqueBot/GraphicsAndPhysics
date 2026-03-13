// #version 440 core // hide this on compile lmaoooo.

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;


uniform sampler2D u_albedo;
uniform sampler2DArray u_shadowMap;
uniform float u_deltaTime;
uniform uint u_objectId;

    
layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;
layout (location = 2) out vec4 out_litShadow;

#define LIGHT_POINT 0.0
#define LIGHT_SPOT 1.0
#define LIGHT_DIRECTIONAL 2.0
#define LIGHT_AMBIENT 3.0

#define MAX_LIGHT_COUNT 10
#define MAX_SHADOW_COUNT 10
#define SHADOW_MAP_MATRIX_COUNT 6

struct LightData {
	vec4 m_position_type;	// 1, 2, 3, 4
	vec4 m_direction_roll;
	vec4 m_color_power;
	vec4 m_attenuation;
};


struct ShadowData {
    mat4 m_lightMatrix[SHADOW_MAP_MATRIX_COUNT];
    vec4 m_atlasOffsetSize;
    vec4 m_lightTypeShadowId;
};


layout (std140, binding=2) uniform LightBlock {
	// vec3 _pad;                // pad to 16 bytes so next comes at offset 16
	LightData m_lightData[MAX_LIGHT_COUNT];
	int m_lightCount;
};


layout (std140, binding=3) uniform ShadowMapData {
    ShadowData m_shadowData[MAX_SHADOW_COUNT];
	vec4 m_directionalAtlasResAndTexelSize;
	vec4 m_spotAtlasResAndTexelSize;
	vec4 m_pointAtlasResAndTexelSize;
    int m_shadowCount;
};




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

float CalculateShadow() {

    // for (int i= 0; i < )
    // float shadowLowest = 1.0;
    // vec2 fbSizeF = vec2(m_framebufferSize);
    // vec2 texelSize = vec2(1.0 / fbSizeF.x, 1.0 / fbSizeF.y);

    // for (int i = 0; i < m_count; ++i) {
    //     // sample from the corresponding light map.
    //     LightData currentLight = m_lightData[i];
    //     int layerid = int(currentLight.m_shadowId.x);
    //     if (-1 == layerid) continue; // set to -1 in cpu before sent to gpu if illegal, otherwise it's unsigned::max()
    //     // texture(u_shadow, vec3(frag_uv,))
    //     vec4 lightspacepos = currentLight.m_lightMatrix * vec4(frag_position, 1.0);
        
    //     vec3 projCoords = ((lightspacepos.xyz/lightspacepos.w) + 1.0) * 0.5;
    //     // skipping any out.
    //     if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
    //         projCoords.y < 0.0 || projCoords.y > 1.0 ||
    //         projCoords.z > 1.0
    //     ) {
    //         continue;
    //     }

    //     float currentDepth = projCoords.z;
    //     float bias = 0.005;
    //     float accShadowVal = 0;

    //     vec2 textureCoords = projCoords.xy;
    //     vec2 ratio = vec2(m_baseTileSize)/vec2(m_framebufferSize); 
    //     textureCoords *= ratio;
    //     accShadowVal = PercentageCloserFilter(textureCoords, texelSize, layerid, 5, currentDepth, bias, u_shadowMap);

    //     shadowLowest = min(shadowLowest, accShadowVal);
    // }
    // return shadowLowest;
    return 1.0;

}


vec3 CalculateLighting(vec3 fragPosition, vec3 fragNormal) {
    // return vec3(0.8745, 0.7255, 0.0588);
    vec3 N = normalize(fragNormal);
    
    vec3 result = vec3(0.0);
    vec3 ambientRes = vec3(0.0);
    for (int i = 0; i < m_lightCount; ++i) {
        LightData currentData = m_lightData[i];
        int lightType = int(currentData.m_position_type.w);
        // point light
        if (lightType== LIGHT_POINT) {
            vec3 lightPosition = currentData.m_position_type.xyz;
            vec3 lightVec = fragPosition - lightPosition;
            vec3 nLightVec = normalize(lightVec);
            // we need distance and attenuation values as well to make this work.
            float dist = length(lightVec);
            float power = currentData.m_color_power.w / (dist * dist); // quadratic loss.
            // assumes distance falls off immediately for now.

            float NdotL = max(dot(N, -nLightVec), 0.0);
            vec3 lightColor = currentData.m_color_power.xyz;
            result += lightColor * power * NdotL;
        }


        // directional
        else if (lightType == LIGHT_DIRECTIONAL) {
            // Direction is assumed to be the light direction (e.g., from which light comes)
            vec3 lightDir = normalize(currentData.m_direction_roll.xyz);
            float NdotL = max(dot(N, -lightDir), 0.0);

            vec3 lightColor = currentData.m_color_power.xyz;
            float power = currentData.m_color_power.w;

            // Lambert: albedo * lightColor * power * cos(theta)
            result += lightColor * power *  NdotL;
        }

        // ambient
        else if (lightType == LIGHT_AMBIENT) {
            vec3 lightColor = currentData.m_color_power.xyz;
            float power = currentData.m_color_power.w;

            ambientRes += vec3(lightColor * power);
        }
    }
    ambientRes = clamp(ambientRes, 0.0, 1.0);
    // result *= 1;
    return result * CalculateShadow() + ambientRes;
}

void main() {
	vec4 color = texture(u_albedo, frag_uv);
    out_objectId = u_objectId;
    // out_color = float(u_objectId % 256u) / 255.0; // testing
    out_color = color * vec4(
        CalculateLighting(frag_position, frag_normal), 1.0 // no alpha needed
    );
    // out_color = vec4(vec3(CalculateShadow()), 1.0);

    
}


