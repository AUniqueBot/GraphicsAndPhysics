
#define MAX_LIGHTS 20

struct LightData {
    vec3  m_position;
    float m_intensity;
    vec3  m_color;
    int   m_type; // 1 - direction, 2 - ambient, 3 - point
};
/*
    layout(std140, binding = 1) uniform LightsUBO {
        int lightCount;
        LightData lights[MAX_LIGHTS];
    };
*/