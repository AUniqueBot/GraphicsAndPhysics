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

const glm::vec3& Light::GetColor() const { return m_color; }


void Light::SetColor(const glm::vec3& _col) {
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
    m_castShadowDirty = true;
}
 
void Light::InvalidateShadowMapID() const {
    m_shadowMapID = std::numeric_limits<unsigned>::max();
    m_lightDataMismatch = true;  
}

void Light::SetShadowMapID(unsigned _id) const {
    if (_id == m_shadowMapID) return;
    m_shadowMapID = _id;
    m_lightDataMismatch = true;
} 

unsigned Light::GetShadowMapID() const {
    return m_shadowMapID; 
}

bool Light::CastShadowDirty() const {
    return m_castShadowDirty; 
}

void Light::CleanCastShadow() const {
    m_castShadowDirty = false;
}

const ShadowData& Light::GetShadowData() const {
    if (m_shadowDataMismatch) {
        UpdateShadowData();
    }
    return m_shadowData;
}

ShadowData& Light::GetShadowData() {
    if (m_shadowDataMismatch) {
        UpdateShadowData();
    }
    return m_shadowData;
}

void Light::SetShadowDataMatrix(int level, const glm::mat4& matrix) const {
    m_shadowData.SetMatrix(matrix, level);
}

void Light::SetShadowDataAtlasOffset(int level, const glm::vec2& offset) const {
    m_shadowData.SetAtlasOffset(offset, level);
}

void Light::SetShadowDataAtlasSize(int level, const glm::vec2& size) const {
    m_shadowData.SetAtlasSize(size, level);
}

const LightData& Light::GetLightData() const {
    if (m_lightDataMismatch) {
        UpdateLightData();
    }
    return m_lightData;
}


LightData& Light::GetLightData() {
    if (m_lightDataMismatch) {
         UpdateLightData();
    }
    return m_lightData;
}

void Light::RenderShadow(const MeshRenderer& _mr, const glm::mat4& _objectMatrix) const {
    
    const LightData lightData{ GetLightData() };
     
    const unsigned UNIFORMLOC{};
    glUniformMatrix4fv(UNIFORMLOC, 1, GL_FALSE, glm::value_ptr(_objectMatrix));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_mr.GetMesh()->GetIndexDataCount() * 3), GL_UNSIGNED_INT, 0);
    

}

void Light::UpdateLightData() const {
    m_lightData.SetType(static_cast<float>(m_lightType));
    m_lightData.SetColor(m_color); 
    m_lightData.SetPower(m_power);
    m_lightDataMismatch = false;
}

void Light::UpdateShadowData() const {
    

    m_shadowDataMismatch = false;
}




std::vector<PropertyMD::Property>& Light::GetProps() {
    using namespace PropertyMD;
    static std::vector<Property> props{
        PropertyMD::MakeProperty<Light>("Color", PropertyType::Color, Shape::FixedArray, 3, &Light::GetColor, &Light::SetColor),
        PropertyMD::MakeProperty<Light>("Power", PropertyType::Float, Shape::Scalar, 1,
            static_cast<float (Light::*)() const>(&Light::Power),   // getter
            static_cast<void (Light::*)(float)>(&Light::Power),      // setter
            true
            ),
        PropertyMD::MakeProperty<Light>("Casts Shadow", PropertyType::Boolean, Shape::Scalar, 3, &Light::GetCastShadow, &Light::SetCastShadow),
    };
    return props;
}