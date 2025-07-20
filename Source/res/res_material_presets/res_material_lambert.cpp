#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <util/util_serialisation.h>
#include <util/util_convenient_conversions.h>




void LambertMaterial::Init() {


    // - loading shader ------------------
    std::string vertexShaderSource   = ShaderProgram::ParseShaderCode("./Assets/Shaders/vtx_vertex.vert");
    std::string fragmentShaderSource = ShaderProgram::ParseShaderCode("./Assets/Shaders/frag_flatColor.frag");
    ShaderProgram lambertShader{};
    GLuint vtxShaderId = ShaderProgram::LoadShader(vertexShaderSource.c_str(), ShaderProgram::VERTEX);
    GLuint fragShaderId = ShaderProgram::LoadShader(fragmentShaderSource.c_str(), ShaderProgram::FRAG);

    std::vector<GLuint> shaderList{ vtxShaderId, fragShaderId };
    GLuint programId             { ShaderProgram::LinkShaders(shaderList) };
    lambertShader.SetShaderID(programId);
    SetShaderProgram(std::make_shared<ShaderProgram>(lambertShader));

    // - setting up uniforms -------------
    InitUniformLocations();
}

const glm::vec4& LambertMaterial::Color() const {
    return m_color;
}

void LambertMaterial::Color(glm::vec4 _newColor) {
    m_color = _newColor;
}

void LambertMaterial::Color(unsigned _newColor) {
    m_color = HexToVec4(_newColor);
}





