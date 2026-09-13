#include <arch/core.h>
#include <arch/resources/res_material_presets/res_material_phong.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>
#include <arch/resources/res_textureManager.h>



void PhongMaterialRes::InitInternal() {

    // - setting up uniforms -------------
    InitUniformLocations();
    SetupTextures();
}

void PhongMaterialRes::ResolveUniformValues() {
    if (!m_uniformDataDirty) return;
    m_uniformDataDirty = false;

    using namespace ShaderUniformConstants;
    m_materialValues[U_ALBEDO]->SetValue(GetColorTextureID());
    m_materialValues[U_SPECULAR]->SetValue(GetSpecularTextureID());
    m_materialValues[U_GLOSS]->SetValue(GetGlossTextureID());

    m_materialValues[U_EXPONENT]->SetValue(m_exponentVal);

}



Materials::ShadingModel PhongMaterialRes::GetShadingModel() const { 
    return Materials::ShadingModel::PHONG; 
}

const glm::vec4& PhongMaterialRes::Color() const {
    return m_color;
}


void PhongMaterialRes::Color(const glm::vec4& _newColor) {
    if (m_color == _newColor) return;
    m_color = _newColor;
    if (m_textureColor.HandleIsValid()) {
        m_textureColor->SetPixelColor(_newColor, 0, 0, 0);
    }
}

void PhongMaterialRes::Color(unsigned _newColor) {
    Color(Color::HexToVec4F(_newColor));
}

const GLuint& PhongMaterialRes::GetColorImageTexture() const {
    return m_reservedColorImageTexId;
}

void PhongMaterialRes::SetColorImageTexture(GLuint _textureId) {
    m_reservedColorImageTexId = _textureId;
}

void PhongMaterialRes::SetUsesColorValue(bool _usesColor) {
    if (_usesColor == m_usesColorValue) return;
    m_usesColorValue = _usesColor;
    m_uniformDataDirty = true;
}

bool PhongMaterialRes::UsesColorValue() const {
    return m_usesColorValue;
}




// -----------------------------------------------------------------------------
const glm::vec4& PhongMaterialRes::Specular() const {
    return m_specularCol;
}

void PhongMaterialRes::Specular(const glm::vec4& _newValue) {
    if (m_specularCol == _newValue) return;
    m_specularCol = _newValue;
    if (m_textureSpecular.HandleIsValid()) {
        m_textureSpecular->SetPixelColor(m_specularCol, 0,0,0);
    }

}

void PhongMaterialRes::Specular(unsigned _newColor) {
    Specular(Color::HexToVec4F(_newColor));
}

const GLuint& PhongMaterialRes::GetSpecularImageTexture() const {
    return m_reservedSpecularImageTexId;
}

void PhongMaterialRes::SetUsesSpecularValue(bool _usesSpecularValue) {
    if (_usesSpecularValue == m_usesSpecularValue) return;
    m_usesSpecularValue = _usesSpecularValue;
    m_uniformDataDirty = true;
}

bool PhongMaterialRes::UsesSpecularValue() const {
    return m_usesSpecularValue;
}




// -----------------------------------------------------------------------------
const float& PhongMaterialRes::Gloss() const {
    // TODO: insert return statement here
    return m_glossVal;
}

void PhongMaterialRes::Gloss(float _value) {
    m_glossVal = std::clamp(_value, 0.0f, 1.0f);
    if (m_textureGloss.HandleIsValid()) {
        m_textureGloss->SetPixelColor(glm::vec4(m_glossVal, m_glossVal, m_glossVal, 1.0f), 0, 0, 0);
    }
}

const GLuint& PhongMaterialRes::GetGlossImageTexture() const {
    return m_reservedGlossTexId;
}

void PhongMaterialRes::SetGlossImageTexture(const GLuint& _texture) {
    m_reservedGlossImageTexId = _texture;
}

void PhongMaterialRes::SetSpecularImageTexture(const GLuint& _texture) {
    m_reservedSpecularImageTexId = _texture;
}

GLuint PhongMaterialRes::GetColorTextureID() const {
    return m_usesColorValue ? m_textureColor->GetTextureHandle() : m_reservedColorImageTexId;
}

GLuint PhongMaterialRes::GetSpecularTextureID() const {
    return m_usesSpecularValue ? m_textureSpecular->GetTextureHandle() : m_reservedSpecularImageTexId;
}

GLuint PhongMaterialRes::GetGlossTextureID() const {
    return m_usesGlossValue ? m_textureGloss->GetTextureHandle() : m_reservedGlossImageTexId;
}

void PhongMaterialRes::SetUsesGlossValue(bool _usesGlossValue) {
    if (_usesGlossValue == m_usesGlossValue) return;
    m_usesGlossValue = _usesGlossValue;
    m_uniformDataDirty = true;
}

bool PhongMaterialRes::UsesGlossValue() const {
    return m_usesGlossValue;
}

void PhongMaterialRes::Exponent(int _expVal) {
    if (_expVal == m_exponentVal) return;
    m_exponentVal = _expVal;
    m_uniformDataDirty = true;
}

const int& PhongMaterialRes::Exponent() const {
    return m_exponentVal;
}


void PhongMaterialRes::SetupTextures() {

    Core& c = Core::GetInstance();
    TextureManager& texManager = c.GetAssetManager().GetTextureManager();
    GPUResourceManager& gpuMgr = c.GetGPUResourceManager();
    
    using namespace TextureProperties;
    TextureProps colProps;

    // - generating textures ---------------------------------------------
    std::shared_ptr<TextureRes> ptr;
    colProps.m_internalImageFormat = TextureFormat::RGBA8;
    m_textureColor = texManager.Create2DTexture(1, 1, colProps);
    ptr = m_textureColor.Get();
    ptr->SetGPUResourceHandle(gpuMgr.CreateTexture(*ptr));
    m_textureColor->SetPixelColor(m_color, 0, 0, 0);

    TextureProps specProps;
    specProps.m_internalImageFormat = TextureFormat::RGBA8;
    m_textureSpecular = texManager.Create2DTexture(1, 1, specProps);
    ptr = m_textureSpecular.Get();
    ptr->SetGPUResourceHandle(gpuMgr.CreateTexture(*ptr));
    m_textureSpecular->SetPixelColor(m_specularCol, 0, 0, 0);


    TextureProps glossProps;
    glossProps.m_internalImageFormat = TextureFormat::R8;
    m_textureGloss = texManager.Create2DTexture(1, 1, glossProps);
    ptr = m_textureGloss.Get();
    ptr->SetGPUResourceHandle(gpuMgr.CreateTexture(*ptr));
    m_textureGloss->SetPixelColor(glm::vec4(m_glossVal, m_glossVal, m_glossVal, 1.0f), 0, 0, 0);


    // - setting values --------------------------------------------------
    MaterialValueData matValue;
    matValue.m_type = MaterialValueData::ValueType::Texture;
    matValue.SetValue(GetColorTextureID());
    m_materialValues.add((MaterialValueData{ matValue }), ShaderUniformConstants::U_ALBEDO);

    matValue.m_type = MaterialValueData::ValueType::Texture;
    matValue.SetValue(GetSpecularTextureID());
    m_materialValues.add((MaterialValueData{ matValue }), ShaderUniformConstants::U_SPECULAR);

    matValue.m_type = MaterialValueData::ValueType::Texture;
    matValue.SetValue(GetGlossTextureID());
    m_materialValues.add((MaterialValueData{ matValue }), ShaderUniformConstants::U_GLOSS);


    MaterialValueData expVal;
    expVal.m_type = MaterialValueData::ValueType::Int;
    expVal.SetValue(m_exponentVal);
    m_materialValues.add((MaterialValueData{}), ShaderUniformConstants::U_EXPONENT);
}
 

std::vector<PropertyMD::Property>& PhongMaterialRes::GetProps() {
    using namespace PropertyMD;
    static std::vector<Property> props{
        PropertyMD::MakeProperty<PhongMaterialRes>(
            "Color", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (PhongMaterialRes::*)() const>(&PhongMaterialRes::Color),
            static_cast<void(PhongMaterialRes::*)(const glm::vec4&)>(&PhongMaterialRes::Color)
        ),
        PropertyMD::MakeProperty<PhongMaterialRes>(
            "Specular", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (PhongMaterialRes::*)() const>(&PhongMaterialRes::Specular),
            static_cast<void(PhongMaterialRes::*)(const glm::vec4&)>(&PhongMaterialRes::Specular)
        ),
        PropertyMD::MakeProperty<PhongMaterialRes>(
            "Gloss", PropertyType::Float, PropertyMD::Shape::Scalar, 1,
            static_cast<const float& (PhongMaterialRes::*)() const>(&PhongMaterialRes::Gloss),
            static_cast<void(PhongMaterialRes::*)(float)>(&PhongMaterialRes::Gloss),
            true
        ),
        PropertyMD::MakeProperty<PhongMaterialRes>(
            "Exponent", PropertyType::Int, PropertyMD::Shape::Scalar, 1,
            static_cast<const int& (PhongMaterialRes::*)() const>(&PhongMaterialRes::Exponent),
            static_cast<void(PhongMaterialRes::*)(int)>(&PhongMaterialRes::Exponent),
            true
        ),
    };
    return props;
}