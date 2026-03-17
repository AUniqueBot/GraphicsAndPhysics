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
    SetShaderProgram(std::make_shared<ShaderProgram>(phongShader));

    m_reservedColorTexId = GenerateEmptyColorTexture();

    // - setting up uniforms -------------
    InitUniformLocations();

    UpdateTextureID();
}


const glm::vec4& PhongMaterial::Color() const {
    return m_color;
}


void PhongMaterial::Color(glm::vec4 _newColor) {
    if (m_color == _newColor) return;
    m_color = _newColor;
    UpdateColorTexture(m_reservedColorTexId, m_color);
}

void PhongMaterial::Color(unsigned _newColor) {
    Color(HexToVec4(_newColor));
}

void PhongMaterial::UsesColor(bool _usesColor) {
    if (_usesColor == m_usesColor) return;
    m_usesColor = _usesColor;
    UpdateTextureID();
}

bool PhongMaterial::UsesColor() const {
    return m_usesColor;
}


void PhongMaterial::UpdateTextureID() {
    m_texId = m_usesColor ? m_reservedColorTexId : m_reservedImageTexId;
}


void PhongMaterial::ApplyUniforms() const {
    if (m_uniformLocations.contains(U_ALBEDO)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glUniform1i(m_uniformLocations.at(U_ALBEDO), 0);
    }

}

