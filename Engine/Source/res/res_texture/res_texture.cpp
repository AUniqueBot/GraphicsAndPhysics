#include <pch.h>
#include <arch/resources/res_texture/res_texture.h>
#include <stb_image.h>

// MUST use opengl ver 4.5+ for DSA.

namespace TextureCreation {
	void AllocateTexture(Texture& _texture) {

	}
}



Texture::Texture(TextureProperties::TextureType _type) {
	using namespace TextureProperties;
	TextureProps props{};
	m_textureType = _type;
	m_dimensions = glm::ivec3(1, 1, 1);
	props.m_filterMag = props.m_filterMin = FilterBehaviour::LINEAR;
	props.m_wrapU = props.m_wrapV = WrapBehaviour::REPEAT;

 
	Create();
}

Texture::Texture(TextureProperties::TextureProps _props) {
	m_textureProperties = _props;

	Create();
}

Texture::~Texture() {
	glDeleteTextures(1, &m_textureId);
}

Texture::Texture(Texture&& _old) noexcept {
	m_textureId = _old.m_textureId;
	m_textureProperties = _old.m_textureProperties;
	m_dimensions = _old.m_dimensions;

}

Texture& Texture::operator=(Texture&& _old) noexcept {
	m_textureId = _old.m_textureId;
	m_textureProperties = _old.m_textureProperties;
	m_dimensions = _old.m_dimensions;
	return *this;
}

const GLuint& Texture::GetTextureID() const {
	return m_textureId;
}

const TextureProperties::TextureType& Texture::GetTextureType() const {
	return m_textureType;
}

const TextureProperties::InternalImageFormat& Texture::GetInternalImageFormat() const {
	return m_textureProperties.m_internalImageFormat;
}

void Texture::SetInternalImageFormat(const TextureProperties::InternalImageFormat& _format) {
	if (_format == m_textureProperties.m_internalImageFormat) return;
	m_textureProperties.m_internalImageFormat = _format;
	m_reallocateDirty = true;
}

const TextureProperties::PixelDataType& Texture::GetPixelDataType() const {
	return m_textureProperties.m_pixelDataType;
}

void Texture::SetPixelDataType(const TextureProperties::PixelDataType& _pixelDataType) {
	if (_pixelDataType == m_textureProperties.m_pixelDataType) return;
	m_textureProperties.m_pixelDataType = _pixelDataType;
	m_reallocateDirty = true;
}

const TextureProperties::PixelFormat& Texture::GetPixelFormat() const {
	return m_textureProperties.m_pixelFormat;
}

void Texture::SetPixelFormat(const TextureProperties::PixelFormat& _pixelFormat) {
	if (_pixelFormat == m_textureProperties.m_pixelFormat) return;
	m_textureProperties.m_pixelFormat = _pixelFormat;
	m_reallocateDirty = true;
}

void Texture::SetWrapBehaviourU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {

}

const TextureProperties::WrapBehaviour& Texture::GetWrapBehaviourU() const {
	return m_textureProperties.m_wrapU;
}

void Texture::SetWrapBehaviourV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
}

const TextureProperties::WrapBehaviour& Texture::GetWrapBehaviourV() const {
	return m_textureProperties.m_wrapV;
}

void Texture::SetFilterBehaviourMin(const TextureProperties::FilterBehaviour& _filterBehaviour) {
}

const TextureProperties::FilterBehaviour& Texture::GetFilterBehaviourMin() const {
	return m_textureProperties.m_filterMin;
}

void Texture::SetFilterBehaviourMag(const TextureProperties::FilterBehaviour& _filterBehaviour) {
}

const TextureProperties::FilterBehaviour& Texture::GetFilterBehaviourMag() const {
	return m_textureProperties.m_filterMag;
}

void Texture::UpdateTextureProperties() {
	if (!m_samplingDirty) return;
	glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_textureProperties.m_filterMag));
	glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_textureProperties.m_filterMin));
	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_textureProperties.m_wrapU));
	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_textureProperties.m_wrapV));
}

void Texture::Create() {
	if (m_textureId != 0) return;
	glCreateTextures(static_cast<GLenum>(m_textureType), 1, &m_textureId);
	assert(m_textureId != 0 && "Failed to create texture.");
}

void Texture::Destroy() {
	if (m_textureId == 0) return;
	glDeleteTextures(1, &m_textureId);
	m_textureId = 0;
	m_allocated = false;
	m_uploaded = false;
}

void Texture::Allocate() {
	using namespace TextureProperties;
	TextureType textureType = m_textureType;
	int uploadDimensionCount = 0;

	switch (textureType) {
	case TextureType::TEXTURE_1D:
		uploadDimensionCount = 1;
		break;
	case TextureType::TEXTURE_1D_ARRAY:
	case TextureType::TEXTURE_2D:
	case TextureType::CUBEMAP:
		uploadDimensionCount = 2;
		break;

	case TextureType::TEXTURE_2D_ARRAY:
	case TextureType::TEXTURE_3D:
	case TextureType::CUBEMAP_ARRAY:
		uploadDimensionCount = 3;
		break;
	}

	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int depth = m_dimensions.y;
	GLenum internalFormat = (GLenum)m_textureProperties.m_internalImageFormat;
	int mipCount = 0;
	switch (uploadDimensionCount) {
	case 1:
		glTextureStorage1D(m_textureId, mipCount, internalFormat, width);
		break;
	case 2:
		glTextureStorage2D(m_textureId, mipCount, internalFormat, width, height);
		break;
	case 3:
		glTextureStorage3D(m_textureId, mipCount, internalFormat, width, height, depth);
		break;
	}
	m_allocated = true;
}	
