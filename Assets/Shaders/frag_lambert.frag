in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;


uniform sampler2D u_albedo;
uniform sampler2DArrayShadow u_shadowMap;
uniform float u_deltaTime;
uniform uint u_objectId;

    
layout (location = 0) out vec4 out_color;
layout (location = 1) out uint out_objectId;

#define LIGHT_POINT 0.0
#define LIGHT_SPOT 1.0
#define LIGHT_DIRECTIONAL 2.0
#define LIGHT_AMBIENT 3.0


struct LightData {
	vec4 m_position_type;	// 1, 2, 3, 4
	vec4 m_direction_roll;
	vec4 m_color_power;
	vec4 m_attenuation;
    mat4 m_lightMatrix;
    vec4 m_shadowId;
};
layout (std140, binding=2) uniform LightBlock {
	// vec3 _pad;                // pad to 16 bytes so next comes at offset 16
	LightData m_lightData[10];
	int m_count;
};


float CalculateShadow() {
    float shadowLowest = 1.0;
    for (int i = 0; i < m_count; ++i) {
        // sample from the corresponding light map.
        LightData currentLight = m_lightData[i];
        int layerid = int(currentLight.m_shadowId.x);
        if (-1 == layerid) continue; // set to -1 in cpu before sent to gpu if illegal, otherwise it's unsigned::max()
        // texture(u_shadow, vec3(frag_uv,))

        vec4 lightspacepos = currentLight.m_lightMatrix * vec4(frag_position, 1.0);
        vec3 projCoords = (lightspacepos.xyz/lightspacepos.w + 1.0) * 0.5;
        float closestDepth = texture(u_shadowMap, vec3(projCoords.xy, layerid)).r;
        float currentDepth = projCoords.z;
        float bias = 0.005;
        float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
        shadowLowest = min(shadow, shadowLowest);
    }
    return shadowLowest;
}


vec3 CalculateLighting(vec3 fragPosition, vec3 fragNormal) {
    // return vec3(0.8745, 0.7255, 0.0588);
    vec3 N = normalize(fragNormal);
    
    vec3 result = vec3(0.0);
    vec3 ambientRes = vec3(0.0);
    for (int i = 0; i < m_count; ++i) {
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
            float NdotL = max(dot(N, lightDir), 0.0);

            vec3 lightColor = currentData.m_color_power.xyz;
            float power = currentData.m_color_power.w;

            // Lambert: albedo * lightColor * power * cos(theta)
            result += lightColor * power * NdotL;
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

    
}


