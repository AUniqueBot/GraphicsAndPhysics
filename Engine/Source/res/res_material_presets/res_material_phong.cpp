#include <arch/resources/res_material_presets/res_material_phong.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>



void PhongMaterial::Init() {


    // - loading shader ------------------
    std::string vertexShaderSource = "#version 460 core\n" + ShaderUtilFunctions::ParseShaderCode("./Assets/Shaders/vtx_vertex.vert");
    std::string fragmentShaderSource = "#version 460 core\n" + ShaderUtilFunctions::ParseShaderCode("./Assets/Shaders/frag_phong.frag");

    ShaderProgram phongShader{};
    Shader vertexShader;
    Shader fragmentShader;

    vertexShader.SetShaderCode(vertexShaderSource);
    vertexShader.SetShaderType(ShaderConstants::ShaderType::VERTEX);
    fragmentShader.SetShaderCode(fragmentShaderSource);
    fragmentShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
    vertexShader.Build();
    fragmentShader.Build();
    phongShader.SetShader(vertexShader, ShaderConstants::ShaderType::VERTEX);
    phongShader.SetShader(fragmentShader, ShaderConstants::ShaderType::FRAG);
    phongShader.Build();
    SetShaderProgram(phongShader.GetShaderProgramID());

    m_reservedColorTexId = GenerateEmptyColorTexture();
    m_reservedSpecularTexId = GenerateEmptyColorTexture();
    m_reservedGlossTexId = GenerateEmptyColorTexture();

    // - setting up uniforms -------------
    InitUniformLocations();

    UpdateTextureID();
}


const glm::vec4& PhongMaterial::GetColor() const {
    return m_color;
}


void PhongMaterial::SetColor(glm::vec4 _newColor) {
    if (m_color == _newColor) return;
    m_color = _newColor;
    UpdateColorTexture(m_reservedColorTexId, m_color);
}

void PhongMaterial::SetColor(unsigned _newColor) {
    SetColor(HexToVec4(_newColor));
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
    UpdateTextureID();
}

bool PhongMaterial::UsesColorValue() const {
    return m_usesColorValue;
}




// -----------------------------------------------------------------------------
const glm::vec4& PhongMaterial::GetSpecular() const {
    return m_specularCol;
}

void PhongMaterial::SetSpecular(const glm::vec4& _newValue) {
    if (m_specularCol == _newValue) return;
    m_specularCol = _newValue;
    UpdateColorTexture(m_reservedSpecularTexId, m_specularCol);
}

void PhongMaterial::SetSpecular(unsigned _newColor) {
    SetSpecular(HexToVec4(_newColor));
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
const float& PhongMaterial::GetGloss() const {
    // TODO: insert return statement here
    return m_glossVal;
}

void PhongMaterial::SetGloss(float _value) {
    m_glossVal = std::clamp(_value, 0.0f, 1.0f);
    UpdateColorTexture(m_reservedGlossImageTexId, glm::vec4(m_glossVal));
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
    return m_usesColorValue ? m_reservedColorTexId : m_reservedColorImageTexId;
}

const GLuint& PhongMaterial::GetSpecularTextureID() const {
    return m_usesSpecularValue ? m_reservedSpecularTexId : m_reservedSpecularImageTexId;
}

const GLuint& PhongMaterial::GetGlossTextureID() const {
    return m_usesGlossValue ? m_reservedGlossTexId : m_reservedGlossImageTexId;
}




void PhongMaterial::ApplyUniforms() const {
    if (m_uniformLocations.contains(U_ALBEDO)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GetColorTextureID());
        glUniform1i(m_uniformLocations.at(U_ALBEDO), 0);
    }
    if (m_uniformLocations.contains(U_SPECULAR)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GetSpecularTextureID());
        glUniform1i(m_uniformLocations.at(U_SPECULAR), 0);
    }
    if (m_uniformLocations.contains(U_GLOSS)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GetSpecularTextureID());
        glUniform1i(m_uniformLocations.at(U_GLOSS), 0);
    }
}

