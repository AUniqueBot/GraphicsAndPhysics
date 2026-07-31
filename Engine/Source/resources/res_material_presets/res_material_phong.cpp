#include <arch/resources/res_material_presets/res_material_phong.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>
#include <arch/systems/sys_render_modules/sys_render_textureManager.h>



void PhongMaterial::InitInternal() {

    // - setting up uniforms -------------
    InitUniformLocations();
    SetupTextures();
}


Materials::ShadingModel PhongMaterial::GetShadingModel() const { 
    return Materials::ShadingModel::PHONG; 
}

const glm::vec4& PhongMaterial::Color() const {
    return m_color;
}


void PhongMaterial::Color(const glm::vec4& _newColor) {
    if (m_color == _newColor) return;
    m_color = _newColor;
    m_textureColor.SetPixelColor(_newColor, 0, 0, 0);
}

void PhongMaterial::Color(unsigned _newColor) {
    Color(HexToVec4F(_newColor));
}

const GLuint& PhongMaterial::GetColorImageTexture() const {
    return m_reservedColorImageTexId;
}

void PhongMaterial::SetColorImageTexture(GLuint _textureId) {
    m_reservedColorImageTexId = _textureId;
}

void PhongMaterial::SetUsesColorValue(bool _usesColor) {
    if (_usesColor == m_usesColorValue) return;
    m_usesColorValue = _usesColor;
}

bool PhongMaterial::UsesColorValue() const {
    return m_usesColorValue;
}




// -----------------------------------------------------------------------------
const glm::vec4& PhongMaterial::Specular() const {
    return m_specularCol;
}

void PhongMaterial::Specular(const glm::vec4& _newValue) {
    if (m_specularCol == _newValue) return;
    m_specularCol = _newValue;
    m_textureSpecular.SetPixelColor(m_specularCol, 0,0,0);

}

void PhongMaterial::Specular(unsigned _newColor) {
    Specular(HexToVec4F(_newColor));
}

const GLuint& PhongMaterial::GetSpecularImageTexture() const {
    return m_reservedSpecularImageTexId;
}

void PhongMaterial::SetUsesSpecularValue(bool _usesSpecularValue) {
    m_usesSpecularValue = _usesSpecularValue;
}

bool PhongMaterial::UsesSpecularValue() const {
    return m_usesSpecularValue;
}




// -----------------------------------------------------------------------------
const float& PhongMaterial::Gloss() const {
    // TODO: insert return statement here
    return m_glossVal;
}

void PhongMaterial::Gloss(float _value) {
    m_glossVal = std::clamp(_value, 0.0f, 1.0f);
    m_textureGloss.SetPixelColor(glm::vec4(m_glossVal, m_glossVal, m_glossVal, 1.0f), 0, 0, 0);
}

const GLuint& PhongMaterial::GetGlossImageTexture() const {
    return m_reservedGlossTexId;
}

void PhongMaterial::SetGlossImageTexture(const GLuint& _texture) {
    m_reservedGlossImageTexId = _texture;
}

void PhongMaterial::SetSpecularImageTexture(const GLuint& _texture) {
    m_reservedSpecularImageTexId = _texture;
}

const GLuint& PhongMaterial::GetColorTextureID() const {
    return m_usesColorValue ? m_textureColor.GetTextureHandle() : m_reservedColorImageTexId;
}

const GLuint& PhongMaterial::GetSpecularTextureID() const {
    return m_usesSpecularValue ? m_textureSpecular.GetTextureHandle() : m_reservedSpecularImageTexId;
}

const GLuint& PhongMaterial::GetGlossTextureID() const {
    return m_usesGlossValue ? m_textureGloss.GetTextureHandle() : m_reservedGlossImageTexId;
}

void PhongMaterial::SetupTextures() {
    if (!m_texManagerReference) {
        LOG_WARN("Skipping Texture Setup");
        return;
    }
    TextureManager& texManager { *m_texManagerReference };
    
    using namespace TextureProperties;
    TextureProps colProps;
    colProps.m_internalImageFormat = TextureFormat::RGBA8;
    m_textureColor = texManager.Create2DTexture(1, 1, colProps);
    m_textureColor.SetPixelColor(m_color, 0, 0, 0);

    TextureProps specProps;
    specProps.m_internalImageFormat = TextureFormat::RGBA8;
    m_textureSpecular = texManager.Create2DTexture(1, 1, specProps);
    m_textureSpecular.SetPixelColor(m_specularCol, 0, 0, 0);


    TextureProps glossProps;
    glossProps.m_internalImageFormat = TextureFormat::R8;
    m_textureGloss = texManager.Create2DTexture(1, 1, glossProps);
    m_textureGloss.SetPixelColor(glm::vec4(m_glossVal, m_glossVal, m_glossVal, 1.0f), 0, 0, 0); 
}
 


void PhongMaterial::ApplyUniforms() const {
    if (m_uniformLocations.contains(U_ALBEDO)) {
        glBindTextureUnit(0, GetColorTextureID());
        glProgramUniform1i(m_shader, m_uniformLocations.at(U_ALBEDO), 0);
    }
    if (m_uniformLocations.contains(U_SPECULAR)) {
        glBindTextureUnit(1, GetSpecularTextureID());
        glProgramUniform1i(m_shader, m_uniformLocations.at(U_SPECULAR), 1);
    }
    if (m_uniformLocations.contains(U_GLOSS)) {
        glBindTextureUnit(2, GetGlossTextureID());
        glProgramUniform1i(m_shader, m_uniformLocations.at(U_GLOSS), 2);
    }
}

std::vector<PropertyMD::Property>& PhongMaterial::GetProps() {
    using namespace PropertyMD;
    static std::vector<Property> props{
        PropertyMD::MakeProperty<PhongMaterial>(
            "Color", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (PhongMaterial::*)() const>(&PhongMaterial::Color),
            static_cast<void(PhongMaterial::*)(const glm::vec4&)>(&PhongMaterial::Color)
        ),
        PropertyMD::MakeProperty<PhongMaterial>(
            "Specular", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (PhongMaterial::*)() const>(&PhongMaterial::Specular),
            static_cast<void(PhongMaterial::*)(const glm::vec4&)>(&PhongMaterial::Specular)
        ),
        PropertyMD::MakeProperty<PhongMaterial>(
            "Gloss", PropertyType::Float, PropertyMD::Shape::Scalar, 1,
            static_cast<const float& (PhongMaterial::*)() const>(&PhongMaterial::Gloss),
            static_cast<void(PhongMaterial::*)(float)>(&PhongMaterial::Gloss),
            true
        ),
    };
    return props;
}