#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <arch/systems/sys_render_modules/sys_render_textureManager.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/core.h>

void LambertMaterial::Init() {
    // - setting up uniforms -------------
    InitUniformLocations();
    SetupTextures();
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
    m_textureColor.SetPixelColor(_newColor, 0, 0, 0);
}

void LambertMaterial::Color(unsigned _newColor) {
    Color(HexToVec4F(_newColor));
}

void LambertMaterial::UsesColor(bool _usesColor) {
    if (_usesColor == m_usesColor) return;
    m_usesColor = _usesColor;
}

bool LambertMaterial::UsesColor() const {
    return m_usesColor;
}

const GLuint& LambertMaterial::GetColorTextureID() const {
    return m_usesColor ? m_textureColor.GetTextureHandle() : m_reservedImageTexId;
}

void LambertMaterial::SetupTextures() {
    if (!m_texManagerReference) {
        LOG_WARN("Skipping Texture Setup");
        return;
    }
    TextureManager& texManager{ *m_texManagerReference };
    using namespace TextureProperties;
    TextureProps props;
    props.m_internalImageFormat = TextureFormat::RGBA8;
    m_textureColor = texManager.Create2DTexture(1, 1);
    m_textureColor.SetPixelColor(m_color, 0, 0, 0);
}




void LambertMaterial::ApplyUniforms() const {
    if (m_uniformLocations.contains(U_ALBEDO)) {
        // would be better if applied in the system instead of over here.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GetColorTextureID());
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