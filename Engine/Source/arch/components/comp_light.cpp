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

const glm::vec3& Light::Color() const { return m_color; }


void Light::Color(glm::vec3 _col) {
    if (m_color == _col) return;
    m_color = _col;
    m_lightDataMismatch = true;
}



bool Light::GetCastShadow() const { 
    return m_castShadow; 
}
void Light::SetCastShadow(bool _cast) {
    if (_cast == m_castShadow) {
        return;
    }
    m_castShadow = _cast; 


    if (_cast && m_lightType != AMBIENT) {
        m_shadowMapRt = std::make_unique<RenderTarget>();
        m_shadowMapRt->Resolution(m_shadowMapResolution);
        m_shadowMapRt->Build();
    }
    else {
        m_shadowMapRt->Destroy();
    }

}

void Light::SetShadowMapResolution(glm::ivec2 _resolution) {
    if (_resolution != m_shadowMapResolution) {
        m_shadowMapResolution = _resolution;
        if (m_shadowMapRt) m_shadowMapRt->Resolution(_resolution);
    }
}

void Light::SetShadowMapResolution(unsigned _width, unsigned _height) {
    SetShadowMapResolution({_width, _height});
}

RenderTarget* Light::GetShadowMap() {
    return m_shadowMapRt.get();
}

const RenderTarget* Light::GetShadowMap() const {
    return m_shadowMapRt.get();
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
