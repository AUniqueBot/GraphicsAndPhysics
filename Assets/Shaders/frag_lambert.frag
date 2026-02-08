in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;


uniform sampler2D u_albedo;
uniform float u_deltaTime;
uniform uint u_objectId;

layout(location = 0) out vec4 out_color;
layout(location = 1) out uint out_objectId;
// out vec4 (location = 1) out_objectId;
/*
*/

struct LightData {
	vec4 m_position_type;	// 1, 2, 3, 4
	vec4 m_direction_roll;
	vec4 m_color_power;
	vec4 m_attenuation;
};

layout (std140, binding=2) uniform LightBlock {
	int m_count;
	// int _pad[3];                // pad to 16 bytes so next comes at offset 16
	LightData m_lightData[10];
};


vec3 CalculateLighting(vec3 fragPosition, vec3 fragNormal) {
    vec3 N = normalize(fragNormal);

    vec3 result = vec3(0.0);
    vec3 ambientRes = vec3(0.0);
    for (int i = 0; i < m_count; ++i) {
        LightData currentData = m_lightData[i];

        // point light
        if (currentData.m_position_type.w == 0.0) {
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
        else if (currentData.m_position_type.w == 3.0) {
            // Direction is assumed to be the light direction (e.g., from which light comes)
            vec3 lightDir = normalize(currentData.m_direction_roll.xyz);
            float NdotL = max(dot(N, lightDir), 0.0);

            vec3 lightColor = currentData.m_color_power.xyz;
            float power = currentData.m_color_power.w;

            // Lambert: albedo * lightColor * power * cos(theta)
            result += lightColor * power * NdotL;
        }

        // ambient
        else if (currentData.m_position_type.w == 4.0) {
            LightData currentData = m_lightData[i];
            if (currentData.m_position_type.w != 4.0) {
                continue;
            }
            vec3 lightColor = currentData.m_color_power.xyz;
            float power = currentData.m_color_power.w;

            ambientRes += vec3(lightColor * power);
        }
    }
    ambientRes = clamp(ambientRes, 0.0, 1.0);
    return result + ambientRes;
}

void main() {
	vec4 color = texture(u_albedo, frag_uv);
    out_objectId = u_objectId;
    // out_color = float(u_objectId % 256u) / 255.0; // testing
    out_color = color * vec4(
        CalculateLighting(frag_position, frag_normal), 1.0 // no alpha needed
    );
}


