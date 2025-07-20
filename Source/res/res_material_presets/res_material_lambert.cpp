#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <util/util_serialisation.h>

void LambertMaterial::Init() {


    // - loading shader ------------------
    std::string vertexShaderSource = FileReading::GetRawTextFromFile("./Assets/Shaders/vtx_vertex.vert");
    std::string fragmentShaderSource = FileReading::GetRawTextFromFile("./Assets/Shaders/frag_flatColor.frag");
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

void LambertMaterial::Render(
    const glm::mat4& _objectMatrix, 
    const glm::mat4& _projectionMatrix, 
    const glm::mat4& _cameraMatrix
) const {
    const int shaderId  { GetShader() };
    if (!shaderId) return;

    glUseProgram(shaderId);



    
    glUniformMatrix4fv(m_uniformLocations.at(OBJECT_MATRIX), 1, GL_FALSE, glm::value_ptr(_objectMatrix));
    glUniformMatrix4fv(m_uniformLocations.at(CAMERA_MATRIX), 1, GL_FALSE, glm::value_ptr(_cameraMatrix));
    glUniformMatrix4fv(m_uniformLocations.at(PROJECTION_MATRIX), 1, GL_FALSE, glm::value_ptr(_projectionMatrix));


}


