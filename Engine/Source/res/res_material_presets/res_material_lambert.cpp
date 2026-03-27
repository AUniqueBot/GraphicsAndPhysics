#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/core.h>

void LambertMaterial::Init() {

    m_reservedColorTexId = GenerateEmptyColorTexture();
    UpdateColorTexture(m_reservedColorTexId, m_color);
    // - setting up uniforms -------------
    InitUniformLocations();
    UpdateTextureID();
}

Materials::ShadingModel LambertMaterial::GetShadingModel() const {
    return Materials::ShadingModel::LAMBERT;
}


const glm::vec4& LambertMaterial::Color() const {
    return m_color;
}


void LambertMaterial::Color(const glm::vec4& _newColor) {
    if (m_color == _newColor) return;
    m_color = _newColor;
    UpdateColorTexture(m_reservedColorTexId, m_color);
}

void LambertMaterial::Color(unsigned _newColor) {
    Color(HexToVec4(_newColor));
}

void LambertMaterial::UsesColor(bool _usesColor) {
    if (_usesColor == m_usesColor) return;
    m_usesColor = _usesColor;
    UpdateTextureID();
}

bool LambertMaterial::UsesColor() const {
    return m_usesColor;
}


void LambertMaterial::UpdateTextureID() {
    m_texId = m_usesColor ? m_reservedColorTexId : m_reservedImageTexId;
}




void LambertMaterial::ApplyUniforms() const {
    if (m_uniformLocations.contains(U_ALBEDO)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glUniform1i(m_uniformLocations.at(U_ALBEDO), 0);
    }
}



std::vector<PropertyMD::Property>& LambertMaterial::GetProps() {
    using namespace PropertyMD;
    static std::vector<Property> props{
        PropertyMD::MakeProperty<LambertMaterial>(
            "Color", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (LambertMaterial::*)() const>(&LambertMaterial::Color),
            static_cast<void(LambertMaterial::*)(const glm::vec4&)>(&LambertMaterial::Color)
        ),
    };
    return props;
}