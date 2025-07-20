#include <arch/resources/res_material.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void Material::Init() {
    
}

void Material::SetShaderProgram(std::shared_ptr<ShaderProgram> shaderProg){
	m_shader = std::move(shaderProg);
}

int Material::GetShader() const {
    return (m_shader.get() == nullptr) ? 0 : m_shader.get()->ShaderID();
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



void Material::InitUniformLocations() {
	if (m_shader.get()) {
		int programId { m_shader.get()->ShaderID() };
		GLint uniformCount{};
		glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

		for (unsigned i{}; i < uniformCount; ++i) {
			std::string name(256, '\0');
			GLint nameLen{};
			GLint size{};
			GLenum type{};
			glGetActiveUniform(programId, i, name.size(), &nameLen, &size, &type, &name[0]);
			name.resize(nameLen);
			

			// data here
			UniformData u_data{};
			u_data.m_uniformLocation = i;
			u_data.m_type = type;
			LOG_INFO("Uniform name - : " + name);
			m_uniformData.emplace(name, u_data);
			m_uniformLocations.emplace(name, i);
		}
	}
}

void Material::SetUniform(std::string _uniformName, UniformData _data) const {
	GLenum dataType{ _data.m_type };
	GLint location{ _data.m_uniformLocation };

	if (location == -1) {
		LOG_WARN(_uniformName + " not found in program, ignoring.");
		return;
	}



	// Lambda to call the correct glUniform* function based on variant type
	auto setter = [&](auto&& value) {
		using T = std::decay_t<decltype(value)>;
		if constexpr (std::is_same_v<T, int>) {
			glUniform1i(_data.m_uniformLocation, value);
			//LOG_INFO("Set int uniform '" + _uniformName + "' to " + std::to_string(value));
		}
		else if constexpr (std::is_same_v<T, unsigned>) {
			glUniform1ui(_data.m_uniformLocation, value);
			//LOG_INFO("Set unsigned int uniform '" + _uniformName + "' to " + std::to_string(value));
		}
		else if constexpr (std::is_same_v<T, float>) {
			glUniform1f(_data.m_uniformLocation, value);
			//LOG_INFO("Set float uniform '" + _uniformName + "' to " + std::to_string(value));
		}
		else if constexpr (std::is_same_v<T, glm::vec2>) {
			glUniform2fv(_data.m_uniformLocation, 1, &value[0]);
			//LOG_INFO("Set vec2 uniform '" + _uniformName + "'");
		}
		else if constexpr (std::is_same_v<T, glm::vec3>) {
			glUniform3fv(_data.m_uniformLocation, 1, &value[0]);
			//LOG_INFO("Set vec3 uniform '" + _uniformName + "'");
		}
		else if constexpr (std::is_same_v<T, glm::vec4>) {
			glUniform4fv(_data.m_uniformLocation, 1, &value[0]);
			//LOG_INFO("Set vec4 uniform '" + _uniformName + "'");
		}
		else if constexpr (std::is_same_v<T, glm::mat2>) {
			glUniformMatrix2fv(_data.m_uniformLocation, 1, GL_FALSE, &value[0][0]);
			//LOG_INFO("Set mat2 uniform '" + _uniformName + "'");
		}
		else if constexpr (std::is_same_v<T, glm::mat3>) {
			glUniformMatrix3fv(_data.m_uniformLocation, 1, GL_FALSE, &value[0][0]);
			//LOG_INFO("Set mat3 uniform '" + _uniformName + "'");
		}
		else if constexpr (std::is_same_v<T, glm::mat4>) {
			glUniformMatrix4fv(_data.m_uniformLocation, 1, GL_FALSE, &value[0][0]);
			//LOG_INFO("Set mat4 uniform '" + _uniformName + "'");
		}
		else {
			//LOG_ERROR("Unsupported uniform type for '" + _uniformName + "'");
		}
		};

	try {
		std::visit(setter, _data.m_data);
	}
	catch (const std::bad_variant_access& e) {
		LOG_ERROR("Variant access error in SetUniform for '" + _uniformName + "': " + e.what());
	}

}

void Material::Render(
	const glm::mat4& _objectMatrix,
	const glm::mat4& _projectionMatrix,
	const glm::mat4& _cameraMatrix
) const {

	const int shaderId{ GetShader() };
	if (!shaderId) return;
	glUseProgram(shaderId);

	for (const auto& [uniformName, data] : m_uniformData) {
		SetUniform(uniformName, data);
	}

	glUniformMatrix4fv(m_uniformData.at(U_OBJECT_MATRIX).m_uniformLocation, 1, GL_FALSE, glm::value_ptr(_objectMatrix));
	glUniformMatrix4fv(m_uniformData.at(U_PROJECTION_MATRIX).m_uniformLocation, 1, GL_FALSE, glm::value_ptr(_projectionMatrix));
	glUniformMatrix4fv(m_uniformData.at(U_CAMERA_MATRIX).m_uniformLocation, 1, GL_FALSE, glm::value_ptr(_cameraMatrix));



}