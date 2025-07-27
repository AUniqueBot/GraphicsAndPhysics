#include <arch/components/comp_light.h>


LightType Light::Type() const {
    return m_lightType;
}

void Light::Type(LightType _type) {
    if (_type == m_lightType) {
        return;
    }
    m_lightType = _type;
    m_lightDataMismatch = true;
}

float Light::Power() const {
    return m_power;
}

void Light::Power(float _value) {
    if (m_power == _value) return;
    m_power = _value;
    m_lightDataMismatch = true;
}

glm::vec3 Light::Color() const {
    return m_color;
}


inline void Light::Color(glm::vec3 _col) {
    if (m_color == _col) return;
    m_color = _col;
    m_lightDataMismatch = true;
}


LightData Light::GetLightData() const {
    if (m_lightDataMismatch) {
         UpdateLightData();
    }
    return m_lightData;
}

void Light::UpdateLightData() const {
    if (!m_lightDataMismatch) return;
    m_lightData.m_position_type.w   = m_lightType;
    m_lightData.m_color_power       = glm::vec4(m_color, m_power);
    m_lightDataMismatch = false;
}
