#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <arch/resources/res_textureManager.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/core.h>

void LambertMaterial::InitInternal() {
    // - setting up uniforms -------------
    InitUniformLocations();
    SetupTextures();
}

void LambertMaterial::ResolveTextureValues() {
    
    if (m_valuesDirty) {
        if (m_textureColor.HandleIsValid()) {
            Color(m_color);
        }
        else {
            
        }
    }
    
    // - uniform values ------------------
    if (m_textureReferenceDirty) {
        m_textureReferenceDirty = false;
        m_materialValues[U_ALBEDO]->SetValue(GetColorTextureID());
    }
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
    m_textureReferenceDirty = true;
}

bool LambertMaterial::UsesColor() const {
    return m_usesColor;
}

GLuint LambertMaterial::GetColorTextureID() const {
    return m_usesColor ? m_textureColor.GetTextureHandle() : m_reservedImageTexId;
}

void LambertMaterial::SetupTextures() {
    TextureManager& texManager{ Core::GetInstance().GetAssetManager().GetTextureManager() };
    using namespace TextureProperties;
    TextureProps props;
    props.m_internalImageFormat = TextureFormat::RGBA8;
    m_textureColor = texManager.Create2DTexture(1, 1, props);
    m_textureColor.SetPixelColor(m_color, 0, 0, 0);
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