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
		m_uniformLocations[OBJECT_MATRIX] = glGetUniformLocation(programId, OBJECT_MATRIX);
		m_uniformLocations[CAMERA_MATRIX] = glGetUniformLocation(programId, CAMERA_MATRIX);
		m_uniformLocations[PROJECTION_MATRIX] = glGetUniformLocation(programId, PROJECTION_MATRIX);
	}
}


