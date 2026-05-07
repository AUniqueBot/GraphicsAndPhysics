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


void Texture::Upload(TextureProperties::TextureUploadData _imageData) {
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int depth = m_dimensions.z;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt; // to replace with a std::vector<void*>

	if (hasExternalData) {
		if (m_textureType != TextureProperties::TextureType::TEXTURE_2D) {
			LOG_INFO("Uploading of non-2D textures is not supported.");
			hasExternalData = false;
			_imageData = std::nullopt;
		}
		if (!autogenMips && _imageData.value().size() != mipCount) {
			throw std::runtime_error("mismatch mip count and image data");
		}
	}

	GLenum internalImageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);




	if (mipCount > 1) {
		void* imageData = nullptr;
		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
			int mipWidth = width >> mipLevel;
			int mipHeight = height >> mipLevel;
			if (hasExternalData) {
				const auto& data = _imageData.value()[mipLevel];
				if (data.m_dimensions.x != mipWidth || data.m_dimensions.y != mipHeight) {
					std::stringstream ss;
					ss << "provided mip width does not match current mip width. \n";
					ss << "provided dimensions: " << glm::ivec2(data.m_dimensions.x, data.m_dimensions.y) << " || ";
					ss << "expected dimensions: " << glm::ivec2(mipWidth, mipHeight) << "\n";
					ss << "current mip level: " << mipLevel;
					throw std::runtime_error(ss.str());
				}
				imageData = data.m_textureData;
			}
			glTextureSubImage2D(
				m_textureId,
				mipLevel, 0, 0,
				mipWidth, mipHeight,
				internalImageFormat,
				pixelType,
				imageData
			);
		}
	}
	else {
		void* imageData = nullptr;
		if (hasExternalData) {
			const auto& data = _imageData.value()[0];
			if (data.m_dimensions.x != width || data.m_dimensions.y != height) {
				std::stringstream ss;
				ss << "provided image width does not match texture width. \n";
				ss << "provided dimensions: " << glm::ivec2(data.m_dimensions.x, data.m_dimensions.y) << " || ";
				ss << "expected dimensions: " << glm::ivec2(width, height) << "\n";
				throw std::runtime_error(ss.str());
			}
			imageData = data.m_textureData;
		}

		glTextureSubImage2D(
			m_textureId,
			0, 0, 0,
			width, height,
			internalImageFormat,
			pixelType,
			imageData
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_textureId);
		}

	}
}
/*
void Texture::Upload1DTextureData(TextureProperties::TextureUploadData _imageData) const {
	int width = m_dimensions.x;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);

	if (hasExternalData) {
		LOG_WARN("Uploading 1D textures not supported");
		_imageData = std::nullopt;
		hasExternalData = false;
	}
	if (mipCount > 1) {
		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
			int mipWidth = width >> mipLevel;
			glTextureSubImage1D(m_textureId, mipLevel, 0, mipWidth, imageFormat, pixelType, nullptr);
		}
	}
	else {
		glTextureSubImage1D(m_textureId, 0, 0, width, imageFormat, pixelType, nullptr);
		if (autogenMips) {
			glGenerateTextureMipmap(m_textureId);
		}
	}
}
*/

void Texture::UploadTexture2DData(TextureProperties::TextureUploadData _imageData) const {

	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);

	if (hasExternalData) {
		if (!autogenMips && _imageData.value().size() != mipCount) {
			throw std::runtime_error("mismatch mip count and image data");
		}
	}
	if (mipCount > 1) {

		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
			int mipWidth = width >> mipLevel;
			int mipHeight = height >> mipLevel;
			void* imageData = nullptr;
			if (hasExternalData) {
				const auto& data = _imageData.value()[mipLevel];
				if (data.m_dimensions.x != mipWidth || data.m_dimensions.y != mipHeight) {
					std::stringstream ss;
					ss << "provided mip width does not match current mip width. \n";
					ss << "provided dimensions: " << glm::ivec2(data.m_dimensions.x, data.m_dimensions.y) << " || ";
					ss << "expected dimensions: " << glm::ivec2(mipWidth, mipHeight) << "\n";
					ss << "current mip level: " << mipLevel;
					throw std::runtime_error(ss.str());
				}
				imageData = data.m_textureData;
			}
			glTextureSubImage2D(
				m_textureId, 
				mipLevel, 0, 0, 
				mipWidth, mipHeight, 
				imageFormat, 
				pixelType, 
				imageData
			);
		}
	}
	else {
		void* imageData = nullptr;
		if (hasExternalData) {
			const auto& data = _imageData.value()[0];
			if (data.m_dimensions.x != width || data.m_dimensions.y != height) {
				std::stringstream ss;
				ss << "provided image width does not match texture width. \n";
				ss << "provided dimensions: " << glm::ivec2(data.m_dimensions.x, data.m_dimensions.y) << " || ";
				ss << "expected dimensions: " << glm::ivec2(width, height) << "\n";
				throw std::runtime_error(ss.str());
			}
			imageData = data.m_textureData;
		}
		glTextureSubImage2D(
			m_textureId, 0, 
			0, 0, 
			width, height, 
			imageFormat, 
			pixelType, 
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_textureId);
		}
	}
}

void Texture::UploadCubemapData(TextureProperties::TextureUploadData _imageData) const {
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);



	if (hasExternalData) {
		int totalExpectedImages = 6 * mipCount;
		if (_imageData.value().size() != totalExpectedImages) {
			throw std::runtime_error("incorrect usage: number of images mismatch.");
		}
		

		LOG_WARN("Uploading 2D textures for cubemaps not supported");
		_imageData = std::nullopt;
		hasExternalData = false;
	}
	if (mipCount > 1) {

		
		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
			int mipWidth = width >> mipLevel;
			int mipHeight = height >> mipLevel;
			glm::ivec2 mipDims = { mipWidth, mipHeight };
			for (int i = 0; i < 6; ++i) {
				
				void* data = nullptr;
				if (hasExternalData) {
					int imageIndex = 6 * mipLevel + i;
					auto& textureData = _imageData.value()[imageIndex];
					// check if cubemap dims match
					glm::ivec2 imageDims = { textureData.m_dimensions.x, textureData.m_dimensions.y };
					
					if (mipDims != imageDims) {
						LOG_ERROR("mip dimension mismatch, assuming texture does not exist.");
					}
					else {
						data = textureData.m_textureData;
					}

					
				}
				glTextureSubImage3D(
					m_textureId, mipLevel,
					0, 0, i,
					mipWidth, mipHeight, 1,
					imageFormat,
					pixelType,
					data
				);
			}
		}
	}
	else {
		
		glm::ivec2 dims = { width, height };
		for (int i = 0; i < 6; ++i) {
			
			void* data = nullptr;
			if (hasExternalData) {
				auto& textureData = _imageData.value()[i];
				// check if cubemap dims match
				glm::ivec2 imageDims = { textureData.m_dimensions.x, textureData.m_dimensions.y };

				if (dims != imageDims) {
					LOG_ERROR("mip dimension mismatch, assuming texture does not exist.");
				}
				else {
					data = textureData.m_textureData;
				}
			}
			glTextureSubImage3D(
				m_textureId, 0,
				0, 0, i,
				width, height, 1,
				imageFormat,
				pixelType,
				data
			);
		}
		if (autogenMips) {
			glGenerateTextureMipmap(m_textureId);
		}
	}
}

void Texture::Upload3DTextureData(TextureProperties::TextureUploadData _imageData) const {
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int depth = m_dimensions.z;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);

	if (hasExternalData) {
		LOG_WARN("Uploading 1D textures not supported");
		_imageData = std::nullopt;
		hasExternalData = false;
	}
	if (mipCount > 1) {
		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
			int mipWidth = width >> mipLevel;
			int mipHeight = height >> mipLevel;
			int mipDepth = depth >> mipLevel;
			glTextureSubImage3D(
				m_textureId, mipLevel,
				0, 0, 0,
				mipWidth, mipHeight, mipDepth,
				imageFormat,
				pixelType,
				nullptr
			);
		}
	}
	else {
		glTextureSubImage3D(
			m_textureId, 0,
			0, 0, 0,
			width, height, depth,
			imageFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_textureId);
		}
	}
}

void Texture::UploadTexture2DArrayData(TextureProperties::TextureUploadData _imageData) const {
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int layerCount = m_dimensions.z;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;
	int layerDepth = 1;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);

	if (hasExternalData) {
		LOG_WARN("Uploading 2D textures for arrays not supported");
		_imageData = std::nullopt;
		hasExternalData = false;
	}

	if (mipCount > 1 && autogenMips) {
		throw std::runtime_error("incorrect usage: mip count specified but mips are autogenerated");
	}

	if (mipCount > 1) {
		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
			int mipWidth = width >> mipLevel;
			int mipHeight = height >> mipLevel;
			glTextureSubImage3D(
				m_textureId, mipLevel,
				0, 0, 0,
				mipWidth, mipHeight, layerCount,
				imageFormat,
				pixelType,
				nullptr
			);
		}
	}

	else {
		glTextureSubImage3D(
			m_textureId, 0,
			0, 0, 0,
			width, height, layerDepth,
			imageFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_textureId);
		}
	}
}
