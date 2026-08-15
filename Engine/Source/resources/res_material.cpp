#include <arch/resources/res_material.h>
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
//#include <stb_image.h>



MaterialValueData::ValueType OpenGL_ShaderTypeToValueType(GLenum type) {
	return
		type == GL_BOOL ? MaterialValueData::ValueType::Bool :
		type == GL_INT ? MaterialValueData::ValueType::Int :
		type == GL_UNSIGNED_INT ? MaterialValueData::ValueType::Unsigned :
		type == GL_FLOAT ? MaterialValueData::ValueType::Float :

		type == GL_FLOAT_VEC2 ? MaterialValueData::ValueType::Vec2 :
		type == GL_FLOAT_VEC3 ? MaterialValueData::ValueType::Vec3 :
		type == GL_FLOAT_VEC4 ? MaterialValueData::ValueType::Vec4 :

		type == GL_INT_VEC2 ? MaterialValueData::ValueType::IVec2 :
		type == GL_INT_VEC3 ? MaterialValueData::ValueType::IVec3 :
		type == GL_INT_VEC4 ? MaterialValueData::ValueType::IVec4 :

		type == GL_FLOAT_MAT4 ? MaterialValueData::ValueType::Mat4f :

		type == GL_SAMPLER_2D ||
		type == GL_SAMPLER_2D_ARRAY ||
		type == GL_SAMPLER_CUBE ||
		type == GL_SAMPLER_2D_SHADOW ||
		type == GL_SAMPLER_2D_ARRAY_SHADOW ? MaterialValueData::ValueType::Texture :

		MaterialValueData::ValueType::INVALID;
}

MaterialValue GetDefaultValue(MaterialValueData::ValueType type) {
	MaterialValue val;
	switch (type) {
	case MaterialValueData::ValueType::Bool:
		val = false;
		break;

	case MaterialValueData::ValueType::Int:
		val = 0;
		break;
	case MaterialValueData::ValueType::Unsigned:
		val = 0u;
		break;
	case MaterialValueData::ValueType::Float:
		val = 0.0f;
		break;
	case MaterialValueData::ValueType::Vec2:
		val = glm::vec2(0.0f);
		break;
	case MaterialValueData::ValueType::Vec3:
		val = glm::vec3(0.0f);
		break;
	case MaterialValueData::ValueType::Vec4:
		val = glm::vec4(0.0f);
		break;
	case MaterialValueData::ValueType::IVec2:
		val = glm::ivec2(0);
		break;
	case MaterialValueData::ValueType::IVec3:
		val = glm::ivec3(0);
		break;

	case MaterialValueData::ValueType::IVec4:
		val = glm::ivec4(0);
		break;

	case MaterialValueData::ValueType::Mat4f:
		val = glm::mat4(1.0f);
		break;


	case MaterialValueData::ValueType::Texture:
		val = 0u;
		break;


	case MaterialValueData::ValueType::INVALID:
	case MaterialValueData::ValueType::_COUNT:
	default:
		val = 0u;
	}
	return val;
}




void Material::Init() {
	InitInternal();
	InitUniformLocations();
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
		//UniformData u_data{};
		//u_data.m_type = type;
		//u_data.m_uniformLocation = location;
		m_uniformLocations.emplace(name, location);

		MaterialValueData::ValueType vt = OpenGL_ShaderTypeToValueType(type);
		m_materialValues.Add({ vt, GetDefaultValue(vt) }, name);

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

void Material::ResolveTextureValues() {
	if (!m_textureReferenceDirty) return;
	m_textureReferenceDirty = false;
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

void Material::ApplyUniforms() {
	ResolveTextureValues();

	unsigned textureUnitId = 0;
	for (const auto& [key, location] : m_uniformLocations) {
		SparseSetView<MaterialValueData> valView = m_materialValues.At(key);
		if (!valView) continue;
		MaterialValueData val { *valView };
		switch (val.m_type) {
		case MaterialValueData::ValueType::Unsigned:
			SetUniformUnsigned(location, val.GetValue<unsigned>());
			break;
		case MaterialValueData::ValueType::Int:
			SetUniformInt(location, val.GetValue<int>());
			break;
		case MaterialValueData::ValueType::Bool:
			SetUniformInt(location, static_cast<int>(val.GetValue<bool>()));
			break;
		case MaterialValueData::ValueType::Float:
			SetUniformFloat(location, val.GetValue<float>());
			break;
		case MaterialValueData::ValueType::Vec2:
			SetUniformVec2(location, val.GetValue<glm::vec2>());
			break;
		case MaterialValueData::ValueType::Vec3:
			SetUniformVec3(location, val.GetValue<glm::vec3>());
			break;
		case MaterialValueData::ValueType::Vec4:
			SetUniformVec4(location, val.GetValue<glm::vec4>());
			break;
		case MaterialValueData::ValueType::IVec2:
			SetUniformIVec2(location, val.GetValue<glm::ivec2>());
			break;
		case MaterialValueData::ValueType::IVec3:
			SetUniformIVec3(location, val.GetValue<glm::ivec3>());
			break;
		case MaterialValueData::ValueType::IVec4:
			SetUniformIVec4(location, val.GetValue<glm::ivec4>());
			break;
		case MaterialValueData::ValueType::Mat4f:
			SetUniformMat4f(location, val.GetValue<glm::mat4>());
			break;
			
		case MaterialValueData::ValueType::Texture:
			SetUniformTextureUnit(location, val.GetValue<GLuint>(), textureUnitId++);
			break;
		}
	}
}



GLint Material::GetUniformLocation(const std::string& _uniformName) const {
	GLint shaderId{ GetShaderProgram() };
	if (!shaderId) return -1;
	return glGetUniformLocation(shaderId, _uniformName.c_str());
}


void Material::SetUniformMat4f(std::string _uniformName, glm::mat4 _value) {
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
	glProgramUniformMatrix4fv(shaderId, uniformLoc, 1, GL_FALSE, glm::value_ptr(_value));
}

void Material::SetUniformVec3(std::string _uniformName, glm::vec3 _value) {
	GLint shaderId{ GetShaderProgram() };
	GLint uniformLoc = -1;
	if (!m_uniformLocations.contains(_uniformName)) {
		uniformLoc = GetUniformLocation(_uniformName);
		// by right this shouldn't happen but jic.
		if (uniformLoc == -1) return;

	}
	glProgramUniform3fv(shaderId, uniformLoc, 1, glm::value_ptr(_value));
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
	glProgramUniform2fv(shaderId, uniformLoc, 1, glm::value_ptr(_value));

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
	glProgramUniform3iv(shaderId, uniformLoc, 1, glm::value_ptr(_value));
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
	glProgramUniform2iv(shaderId, uniformLoc, 1, glm::value_ptr(_value));
	

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
	glProgramUniform1i(shaderId, uniformLoc, _value);

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
	glProgramUniform1ui(shaderId, uniformLoc, _value);
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
	glProgramUniform1f(shaderId, uniformLoc, _value);
}

void Material::SetUniformTextureUnit(std::string _uniformName, GLuint _textureHandle, unsigned _index) {
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
	glBindTextureUnit(_index, _textureHandle);
	glProgramUniform1i(shaderId, uniformLoc, _index);

}



void Material::SetUniformMat4f(GLint _uniformLocation, glm::mat4 _value) const {
	GLint shaderId { GetShaderProgram() };
	glProgramUniformMatrix4fv(shaderId, _uniformLocation, 1, GL_FALSE, glm::value_ptr(_value));
}

void Material::SetUniformVec4(GLint _uniformLocation, glm::vec4 _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform4fv(shaderId, _uniformLocation, 1, glm::value_ptr(_value));
}

void Material::SetUniformVec3(GLint _uniformLocation, glm::vec3 _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform3fv(shaderId, _uniformLocation, 1, glm::value_ptr(_value));
}

void Material::SetUniformVec2(GLint _uniformLocation, glm::vec2 _value) const  {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform2fv(shaderId, _uniformLocation, 1, glm::value_ptr(_value));
}

void Material::SetUniformIVec4(GLint _uniformLocation, glm::ivec4 _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform4iv(shaderId, _uniformLocation, 1, glm::value_ptr(_value));
}

void Material::SetUniformIVec3(GLint _uniformLocation, glm::ivec3 _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform3iv(shaderId, _uniformLocation, 1, glm::value_ptr(_value));
}

void Material::SetUniformIVec2(GLint _uniformLocation, glm::ivec2 _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform2iv(shaderId, _uniformLocation, 1, glm::value_ptr(_value));
}

void Material::SetUniformInt(GLint _uniformLocation, GLint _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform1i(shaderId, _uniformLocation, _value);
}

void Material::SetUniformUnsigned(GLint _uniformLocation, GLuint _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform1ui(shaderId, _uniformLocation, _value);
}

void Material::SetUniformFloat(GLint _uniformLocation, GLfloat _value) const {
	GLint shaderId{ GetShaderProgram() };
	glProgramUniform1f(shaderId, _uniformLocation, _value);
}

void Material::SetUniformTextureUnit(GLint _uniformLocation, GLuint _textureHandle, unsigned _index) const {
	GLint shaderId { GetShaderProgram() };
	glBindTextureUnit(_index, _textureHandle);
	glProgramUniform1i(shaderId, _uniformLocation, _index);
}
