#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>



void LambertMaterial::Init() {


    // - loading shader ------------------
    std::string vertexShaderSource   = "#version 460 core\n" + ShaderProgram::ParseShaderCode("./Assets/Shaders/vtx_vertex.vert");
    std::string fragmentShaderSource = "#version 460 core\n" + ShaderProgram::ParseShaderCode("./Assets/Shaders/frag_lambert.frag");

    ShaderProgram lambertShader{};
    GLuint vtxShaderId = ShaderProgram::LoadShader(vertexShaderSource.c_str(), ShaderProgram::VERTEX);
    GLuint fragShaderId = ShaderProgram::LoadShader(fragmentShaderSource.c_str(), ShaderProgram::FRAG);

    std::vector<GLuint> shaderList{ vtxShaderId, fragShaderId };
    GLuint programId             { ShaderProgram::LinkShaders(shaderList) };
    lambertShader.SetShaderID(programId);
    SetShaderProgram(std::make_shared<ShaderProgram>(lambertShader));


    m_reservedColorTexId = GenerateEmptyColorTexture();

    // - setting up uniforms -------------
    InitUniformLocations();

    UpdateTextureID();
}


const glm::vec4& LambertMaterial::Color() const {
    return m_color;
}


void LambertMaterial::Color(glm::vec4 _newColor) {
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


void LambertMaterial::Render(
    const glm::mat4& _objectMatrix,
    const glm::mat4& _projectionMatrix,
    const glm::mat4& _cameraMatrix
) const {

    //return;
    Material::Render(_objectMatrix, _projectionMatrix, _cameraMatrix);

    if (m_uniformLocations.contains(U_ALBEDO)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glUniform1i(m_uniformLocations.at(U_ALBEDO), 0);
    }
}

