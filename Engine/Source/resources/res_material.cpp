#include <arch/resources/res_material.h>
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
//#include <stb_image.h>


void Material::Init() {
	InitInternal();
	m_initialized = true;
}

void Material::InitInternal() {

}


void Material::SetShaderProgram(GLuint shaderProg){
	m_shader =shaderProg;
}

int Material::GetShaderProgram() const {
	return m_shader;
}

Materials::ShadingModel Material::GetShadingModel() const {
	return Materials::ShadingModel::NONE;
}

void Material::UseMaterial() {
	glUseProgram(GetShaderProgram());
}


void Material::SetTextureManager(TextureManager* _manager) {
	m_texManagerReference = _manager;
}

TextureManager* Material::GetTextureManager() {
	return m_texManagerReference;
}
const TextureManager* Material::GetTextureManager() const {
	return m_texManagerReference;
}


void Material::InitUniformLocations() {
	if (!m_shader) return;
	GLuint programId { m_shader };
	GLint uniformCount{};
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

	for (int i{}; i < uniformCount; ++i) {
		std::string name(256, '\0');
		GLint nameLen{};
		GLint size{};
		GLenum type{};
		glGetActiveUniform(programId, i, static_cast<GLsizei>(name.size()), &nameLen, &size, &type, &name[0]);
		name.resize(nameLen);
		GLint location = glGetUniformLocation(programId, name.c_str());
		if (-1 == location) {
			LOG_WARN("Uniform '" << name << "' location is -1 (probably optimized out or inactive)");
			continue;
		}

		// data here
		UniformData u_data{};
		u_data.m_type = type;
		u_data.m_uniformLocation = location;
		m_uniformData.emplace(name, u_data);
		m_uniformLocations.emplace(name, location);
	}
	
}

void Material::SetUniform(std::string _uniformName, UniformData _data) const {
	GLenum dataType{ _data.m_type };
	GLint location{ _data.m_uniformLocation };

	if (location == -1) {
		LOG_WARN(_uniformName + " not found in program, ignoring.");
		return;
	}
}

bool Material::IsInitialised() const {
	return m_initialized;
}

void Material::SetInitialised(bool _setting) {
	m_initialized = _setting;
}


void Material::ApplyShadowMap(
	const unsigned int& _dirShadowId,
	const unsigned int& _spotShadowId,
	const unsigned int& _pointShadowId

) const {
	GLint uniformLocation{};
	// lambda function
	auto GetUniform = [this](const char* _uniformName) {
		auto it = m_uniformLocations.find(_uniformName);
		if (it == m_uniformLocations.end()) {
			return -1; // invalid location
		}
		return it->second;

		};
	uniformLocation = GetUniform(U_DIRECTIONALSHADOWMAP);

	if (-1 != uniformLocation && _dirShadowId) {
		glBindTextureUnit(31, _dirShadowId);
		glProgramUniform1i(m_shader, uniformLocation, 31);
	}
	uniformLocation = GetUniformLocation(U_POINTSHADOWMAP);
	if (-1 != uniformLocation && _pointShadowId) {
		glBindTextureUnit(30, _pointShadowId);
		glProgramUniform1i(m_shader, uniformLocation, 30);
	}
	uniformLocation = GetUniformLocation(U_SPOTSHADOWMAP);
	if (-1 != uniformLocation && _spotShadowId) {
		glBindTextureUnit(29, _spotShadowId);
		glProgramUniform1i(m_shader, uniformLocation, 29);
	}
}

void Material::ApplyUniforms() const {}



GLint Material::GetUniformLocation(const std::string& _uniformName) const {
	GLint shaderId{ GetShaderProgram() };
	if (!shaderId) return -1;
	return glGetUniformLocation(shaderId, _uniformName.c_str());
}


void Material::SetUniformMatrix(std::string _uniformName, glm::mat4 _value) {
	GLint shaderId{GetShaderProgram()};
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName) ;
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(_value));
}

void Material::SetUniformVec3(std::string _uniformName, glm::vec3 _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;

	}
	glUniform3fv(uniformLoc, 1, glm::value_ptr(_value));

}

void Material::SetUniformVec2(std::string _uniformName, glm::vec2 _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniform2fv(uniformLoc, 1, glm::value_ptr(_value));

}

void Material::SetUniformIVec3(std::string _uniformName, glm::ivec3 _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniform3iv(uniformLoc, 1, glm::value_ptr(_value));
}

void Material::SetUniformIVec2(std::string _uniformName, glm::ivec2 _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniform2iv(uniformLoc, 1, glm::value_ptr(_value));

}

void Material::SetUniformInt(std::string _uniformName, GLint _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniform1i(uniformLoc, _value);

}

void Material::SetUniformUnsigned(std::string _uniformName, GLuint _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniform1ui(uniformLoc, _value);
}

void Material::SetUniformFloat(std::string _uniformName, GLfloat _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;
		m_uniformLocations[_uniformName] = uniformLoc;
	}
	else {
		uniformLoc = m_uniformLocations.at(_uniformName);
	}
	glUniform1f(uniformLoc, _value);
}

