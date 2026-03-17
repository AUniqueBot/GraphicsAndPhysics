#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>



void LambertMaterial::Init() {


    // - loading shader ------------------
    Shader vertexShader     {};
    Shader fragmentShader   {};
    vertexShader.SetShaderCode("#version 460 core\n" + ShaderUtilFunctions::ParseShaderCode("./Assets/Shaders/vtx_vertex.vert"));
    fragmentShader.SetShaderCode("#version 460 core\n" + ShaderUtilFunctions::ParseShaderCode("./Assets/Shaders/frag_lambert.frag"));

    vertexShader.SetShaderType(ShaderConstants::ShaderType::VERTEX);
    fragmentShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
    vertexShader.Build();
    fragmentShader.Build(); 
    ShaderProgram lambertShader{};

    lambertShader.SetShader(vertexShader, ShaderConstants::ShaderType::VERTEX);
    lambertShader.SetShader(fragmentShader, ShaderConstants::ShaderType::FRAG);

    GLuint vtxShaderId = vertexShader.GetShaderID();
    GLuint fragShaderId = fragmentShader.GetShaderID();
    lambertShader.Build();
    //GLuint programId             { ShaderProgram::BuildShaderProgram() };
    //lambertShader.SetShaderProgramID(programId);

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


void LambertMaterial::ApplyUniforms() const {
    if (m_uniformLocations.contains(U_ALBEDO)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glUniform1i(m_uniformLocations.at(U_ALBEDO), 0);
    }
}

