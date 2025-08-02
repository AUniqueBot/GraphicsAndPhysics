in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;
uniform sampler2D u_albedo;
uniform float u_deltaTime;
out vec4 out_color;

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
	LightData m_lightData[50];
};

vec4 CalculateAmbient() {
    vec4 ambientValue = vec4(0);

    for (int i = 0; i < m_count; ++i) {
        LightData currentData = m_lightData[i];
        if (currentData.m_position_type.w != 4.0) {
            continue;
        }
        vec3 lightColor = currentData.m_color_power.xyz;
        float power = currentData.m_color_power.w;

        ambientValue = ambientValue + vec4(lightColor * power, 1.0);
    }

    // Optionally clamp to prevent overbright
    ambientValue = clamp(ambientValue, 0.0, 1.0);

    return ambientValue;
}

void main() {

	vec4 color = texture(u_albedo, frag_uv);
    out_color = color * CalculateAmbient();
}


