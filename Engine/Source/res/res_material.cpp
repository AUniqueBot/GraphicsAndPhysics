#include <arch/resources/res_material.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void Material::Init() {
    
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

unsigned Material::LoadImage(std::string path, bool _hasAlpha, IMAGE_CLAMP_BEHAVIOUR _horizontal, IMAGE_CLAMP_BEHAVIOUR _vertical, FILTER_TYPE _fType) {
	// TODO - Retrofit this function to save as a class for asset management.

	unsigned id{};
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// set the texture wrapping/filtering options (on currently bound texture)
	GLenum hwrap{};
	GLenum vwrap{};
	for (int i{}; i < 2; ++i) {
		GLenum* setting = (i == 0) ? &hwrap : &vwrap;
		IMAGE_CLAMP_BEHAVIOUR* specifier = (i == 0) ? &_horizontal : &_vertical;

		switch (*specifier) {
		case IMAGE_CLAMP_BEHAVIOUR::REPEAT:
			*setting = GL_REPEAT;
			break;
		case IMAGE_CLAMP_BEHAVIOUR::MIRROR_REPEAT:
			*setting = GL_MIRRORED_REPEAT;
			break;
		case IMAGE_CLAMP_BEHAVIOUR::TO_EDGE:
			*setting = GL_CLAMP_TO_EDGE;
			break;
		case IMAGE_CLAMP_BEHAVIOUR::TO_BORDER:
			*setting = GL_CLAMP_TO_BORDER;
			break;
		}


	}
	GLint filter = _fType == FILTER_TYPE::LINEAR ? GL_LINEAR : GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hwrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vwrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	// load and generate the texture
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	GLint format = _hasAlpha ? GL_RGBA : GL_RGB;

	if (!data) {
		LOG_ERROR("Failed to load texture");
		stbi_image_free(data);
		return id;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	LOG_ERROR("Load OK");
	stbi_image_free(data);

	return id;
}

GLuint Material::GenerateEmptyColorTexture() {
	GLuint texId{};
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	GLubyte color[4]{};
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA, 
		1, 1, 
		0, 
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		color
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

GLuint Material::Generate1x1Texture() {
	// WIP function
	// TODOs -> expose image types, expose wrap types

	GLuint texId{};
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	GLubyte color[4]{};
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		1, 1,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		color
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

void Material::DeleteTexture(const GLuint& _id) {
	if (!_id) {
		LOG_INFO("Attempting to delete an unassigned texture");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, _id);
	glDeleteTextures(1,&_id);
}

void Material::UpdateColorTexture(const GLuint& _id, const glm::vec4& _col) {
	if (!_id) {
		LOG_INFO("attempting to update a non existent texture");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, _id);

	GLint width, height{};
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	if (width > 1 || height > 1) {
		LOG_WARN("attempting to bind to a non 1x1 texture");
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0, 0,
		1, 1,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		ColorToBytes(_col).data()
	);


	glBindTexture(GL_TEXTURE_2D, 0);
	const GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR) {
		LOG_ERROR("Bind texture failure.");
	}
	
}

std::array<GLubyte, 4> Material::ColorToBytes(const glm::vec4& col) {
	return {
		static_cast<GLubyte>(col.r * 255.0f),
		static_cast<GLubyte>(col.g * 255.0f),
		static_cast<GLubyte>(col.b * 255.0f),
		static_cast<GLubyte>(col.a * 255.0f)
	};
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

void Material::ApplyShadowMap(const ShadowMap& _shadowMap) const {
	glUseProgram(GetShaderProgram());
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
	if (-1 != uniformLocation) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D_ARRAY, _shadowMap.GetTextureID());
		glUniform1i(uniformLocation, 1);
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

