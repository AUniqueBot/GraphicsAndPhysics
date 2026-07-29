#include <pch.h>
#include <stb_image.h>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/systems/sys_render_modules/sys_render_textureManager.h>

#include <util/util_color.h>

// MUST use opengl ver 4.5+ for DSA.

namespace TextureProperties {

	struct InternalImageDecomposed {
		ImageDataType m_pixelDataType;
		ImageChannels m_pixelFormat;
	};

	// Hidden from headers.
	static InternalImageDecomposed OpenGL_ToDecomposed(TextureFormat _format) {
		InternalImageDecomposed decomposed{};
		switch (_format) {
		case TextureFormat::R8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::Red;
				break;
		case TextureFormat::RG8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RG;
				break;
		case TextureFormat::RGB8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGB;
				break;
		case TextureFormat::RGBA8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGBA;
				break;
		case TextureFormat::SRGB8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGB;
				break;
		case TextureFormat::SRGBA8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGBA;
				break;
		case TextureFormat::R16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::Red;
				break;
		case TextureFormat::RG16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::RG;
				break;
		case TextureFormat::RGB16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::RGB;
				break;
		case TextureFormat::RGBA16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::RGBA;
				break;

		// depth components.
		case TextureFormat::DEPTH24:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::Depth;
				break;
		case TextureFormat::DEPTH32F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::Depth;
			break;
		}

		return decomposed;
	}


	static GLenum OpenGL_ImageFormatToEnum(TextureFormat _format) {
		switch (_format) {
		case TextureFormat::R8: return GL_R8;
		case TextureFormat::RG8: return GL_RG8;
		case TextureFormat::RGB8: return GL_RGB8;
		case TextureFormat::RGBA8: return GL_RGBA8;
		case TextureFormat::R16F: return GL_R16F;
		case TextureFormat::RG16F: return GL_RG16F;
		case TextureFormat::RGB16F: return GL_RGB16F;
		case TextureFormat::RGBA16F: return GL_RGBA16F;
		case TextureFormat::SRGB8: return GL_SRGB8;
		case TextureFormat::SRGBA8: return GL_SRGB8_ALPHA8;
		case TextureFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
		case TextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
		default: return GL_INVALID_ENUM;
		}
	}


	static inline int GetUploadDimension(TextureProperties::TextureType _type) {
		using namespace TextureProperties;
		switch (_type)
		{
		case TextureType::TEXTURE_1D:
			return 1;

		case TextureType::TEXTURE_1D_ARRAY:
		case TextureType::TEXTURE_2D:
			return 2;

		case TextureType::TEXTURE_2D_ARRAY:
		case TextureType::TEXTURE_3D:
		case TextureType::CUBEMAP:
		case TextureType::CUBEMAP_ARRAY:
			return 3;

		default:
			throw std::runtime_error("Unsupported texture type.");
		}
	}

}

namespace {
}

TextureGPU::TextureGPU(
	TextureProperties::TextureType _type, 
	glm::ivec3 _dims, 
	TextureProperties::TextureProps _props
) {
	using namespace TextureProperties;
	m_textureType = _type;
	m_dimensions = _dims;
	m_textureProperties = _props;
	
}



TextureGPU::~TextureGPU() {
	if (!m_glTextureHandle) return;
	LOG_INFO("Destroying Texture with handle [" << m_glTextureHandle << "]");
	glDeleteTextures(1, &m_glTextureHandle);
}

TextureGPU::TextureGPU(TextureGPU&& _old) noexcept {
	m_textureType = _old.m_textureType;
	m_glTextureHandle = _old.m_glTextureHandle;
	m_dimensions = _old.m_dimensions;
	m_textureProperties = _old.m_textureProperties;

	m_allocated = _old.m_allocated;
	m_uploaded = _old.m_uploaded;
	m_uploadNeedUpdate = _old.m_uploadNeedUpdate;
	m_reallocateDirty = _old.m_reallocateDirty;
	m_samplingDirty = _old.m_samplingDirty;



	_old.m_glTextureHandle = 0;
}

TextureGPU& TextureGPU::operator=(TextureGPU&& _old) noexcept {
	m_textureType = _old.m_textureType;
	m_dimensions = _old.m_dimensions;
	m_textureProperties = _old.m_textureProperties;
	m_glTextureHandle = _old.m_glTextureHandle;
	

	m_allocated = _old.m_allocated;
	m_uploaded = _old.m_uploaded;
	m_uploadNeedUpdate = _old.m_uploadNeedUpdate;
	m_reallocateDirty = _old.m_reallocateDirty;
	m_samplingDirty = _old.m_samplingDirty;


	// ensure this one doesn't get it.
	_old.m_glTextureHandle = 0;
	return *this;
}

const GLuint& TextureGPU::GetTextureHandle() const {
	return m_glTextureHandle;
}

const TextureProperties::TextureType& TextureGPU::GetTextureType() const {
	return m_textureType;
}

const TextureProperties::TextureFormat& TextureGPU::GetInternalImageFormat() const {
	return m_textureProperties.m_internalImageFormat;
}

void TextureGPU::SetInternalImageFormat(const TextureProperties::TextureFormat& _format) {
	if (_format == m_textureProperties.m_internalImageFormat) return;
	m_textureProperties.m_internalImageFormat = _format;
	m_reallocateDirty = true;
}

const TextureProperties::ImageDataType& TextureGPU::GetDataType() const {
	return m_textureProperties.m_pixelDataType;
}



void TextureGPU::SetDataType(const TextureProperties::ImageDataType& _pixelDataType) {
	if (_pixelDataType == m_textureProperties.m_pixelDataType) return;
	m_textureProperties.m_pixelDataType = _pixelDataType;
	m_reallocateDirty = true;
}

const TextureProperties::ImageChannels& TextureGPU::GetChannels() const {
	return m_textureProperties.m_pixelFormat;
}

void TextureGPU::SetChannels(const TextureProperties::ImageChannels& _pixelFormat) {
	if (_pixelFormat == m_textureProperties.m_pixelFormat) return;
	m_textureProperties.m_pixelFormat = _pixelFormat;
	m_reallocateDirty = true;
}

void TextureGPU::SetWrapBehaviourU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	if (m_textureProperties.m_wrapU == _wrapBehaviour) return;
	m_textureProperties.m_wrapU = _wrapBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::WrapBehaviour& TextureGPU::GetWrapBehaviourU() const {
	return m_textureProperties.m_wrapU;
}

void TextureGPU::SetWrapBehaviourV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	if (m_textureProperties.m_wrapV == _wrapBehaviour) return;
	m_textureProperties.m_wrapV = _wrapBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::WrapBehaviour& TextureGPU::GetWrapBehaviourV() const {
	return m_textureProperties.m_wrapV;
}

void TextureGPU::SetFilterBehaviourMin(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	if (m_textureProperties.m_filterMin == _filterBehaviour) return;
	m_textureProperties.m_filterMin = _filterBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::FilterBehaviour& TextureGPU::GetFilterBehaviourMin() const {
	return m_textureProperties.m_filterMin;
}

void TextureGPU::SetFilterBehaviourMag(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	if (m_textureProperties.m_filterMag == _filterBehaviour) return;
	m_textureProperties.m_filterMag = _filterBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::FilterBehaviour& TextureGPU::GetFilterBehaviourMag() const {
	return m_textureProperties.m_filterMag;
}

void TextureGPU::SetDimensions(glm::ivec3 _dims) {	
	assert(_dims.x > 0 && _dims.y > 0 && _dims.z > 0 && "attempting to set invalid dimensions");
	if (m_dimensions == _dims) return;
	m_dimensions = _dims;
	m_reallocateDirty = true;
}

const glm::ivec3& TextureGPU::GetDimensions() const {
	return m_dimensions;
}

const int& TextureGPU::GetX() const {
	return m_dimensions.x;
}

const int& TextureGPU::GetY() const {
	return m_dimensions.y;
}

const int& TextureGPU::GetZ() const {
	return m_dimensions.z;
}

void TextureGPU::SetX(const int& _val) {
	assert(_val > 0 && "attempting to set to invalid size");
	if (_val == m_dimensions.x) return;
	m_dimensions.x = _val;
	m_reallocateDirty = true;
}

void TextureGPU::SetY(const int& _val) {
	assert(_val > 0 && "attempting to set to invalid size");
	if (_val == m_dimensions.y) return;
	m_dimensions.y = _val;
	m_reallocateDirty = true;
}

void TextureGPU::SetZ(const int& _val) {
	assert(_val > 0 && "attempting to set to invalid size");
	if (_val == m_dimensions.z) return;
	m_dimensions.z = _val;
	m_reallocateDirty = true;
}

void TextureGPU::SetPixelColor(glm::u8vec1 _col, glm::ivec3 _pixelPos) {
	if (!m_allocated) return;
	int x = _pixelPos.x, y = _pixelPos.y, z = _pixelPos.z;
	using namespace TextureProperties;
	InternalImageDecomposed decomposed = OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);// this controls
	int pixelFormat = static_cast<GLenum>(decomposed.m_pixelFormat);
	int pixelType = static_cast<GLenum>(decomposed.m_pixelDataType);
	int uploadDimCount = GetUploadDimension(m_textureType);
}

void TextureGPU::SetPixelColor(glm::u8vec4 _col, glm::ivec3 _pixelPos) {
	if (!m_allocated) return;
	int x = _pixelPos.x, y = _pixelPos.y, z = _pixelPos.z;
	using namespace TextureProperties;
	InternalImageDecomposed decomposed = OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);
	int pixelFormat = OpenGL::ResolveChannels(decomposed.m_pixelFormat);
	int pixelType = OpenGL::ResolveDataType(decomposed.m_pixelDataType);
	int uploadDimCount = GetUploadDimension(m_textureType);
	
	glm::u8vec4 u8vec4col = {};
	glm::vec4 vec4col = {};

	switch (uploadDimCount) {
	case 1:
		assert(x < m_dimensions.x && x >= 0);
		glTextureSubImage1D(
			m_glTextureHandle, 0, x, 1, 
			pixelFormat,
			pixelType,
			glm::value_ptr(_col)
			// set color here.
		);
		break;
	case 2:
		assert(x < m_dimensions.x && x >= 0);
		assert(y < m_dimensions.y && y >= 0);
		glTextureSubImage2D(
			m_glTextureHandle, 0, x, y, 1, 1,
			pixelFormat,
			pixelType,
			glm::value_ptr(_col)
			// set color here.
		);
		break;
	
	case 3:
		assert(x < m_dimensions.x && x >= 0);
		assert(y < m_dimensions.y && y >= 0);
		assert(z < m_dimensions.z && z >= 0);


		break;
	}

}

void TextureGPU::UpdateTextureProperties() {
	if (!m_samplingDirty) return;
	glTextureParameteri(m_glTextureHandle, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_textureProperties.m_filterMag));
	glTextureParameteri(m_glTextureHandle, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_textureProperties.m_filterMin));
	glTextureParameteri(m_glTextureHandle, GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_textureProperties.m_wrapU));
	glTextureParameteri(m_glTextureHandle, GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_textureProperties.m_wrapV));
	m_samplingDirty = false;
}

void TextureGPU::UpdateAllocation() {
	if (m_allocated && !m_reallocateDirty) return;
	Destroy();
	Create();
	Allocate();
}

void TextureGPU::Create() {
	if (m_glTextureHandle != 0) return;
	glCreateTextures(static_cast<GLenum>(m_textureType), 1, &m_glTextureHandle);
	assert(m_glTextureHandle != 0 && "Failed to create texture.");
}

void TextureGPU::Destroy() {
	if (m_glTextureHandle == 0) return;
	glDeleteTextures(1, &m_glTextureHandle);
	m_glTextureHandle = 0;
	m_allocated = false;
	m_uploaded = false;
}

void TextureGPU::Allocate() {
	using namespace TextureProperties;
	LOG_DEBUG("Allocating for texture handle [" << m_glTextureHandle << "] as " << m_textureType);
	int uploadDimensionCount = GetUploadDimension(m_textureType);

	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int depth = m_dimensions.z;
	GLenum internalFormat = OpenGL_ImageFormatToEnum(m_textureProperties.m_internalImageFormat);
	int mipCount = m_textureProperties.m_mipmapCount > 0 ? m_textureProperties.m_mipmapCount : 1;
	switch (uploadDimensionCount) {
	case 1:
		glTextureStorage1D(m_glTextureHandle, mipCount, internalFormat, width);
		break;
	case 2:
		glTextureStorage2D(m_glTextureHandle, mipCount, internalFormat, width, height);
		break;
	case 3:
		glTextureStorage3D(m_glTextureHandle, mipCount, internalFormat, width, height, depth);
		break;
	}
	m_allocated = true;
}	


void TextureGPU::Upload(TextureProperties::TextureUploadData _imageData) const {
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
		if (!autogenMips && _imageData.has_value() && _imageData.value().size() != mipCount) {
			throw std::runtime_error("mismatch mip count and image data");
		}
	}


	TextureProperties::InternalImageDecomposed decomposed = OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);
	GLenum pixelFormat = static_cast<GLenum>(decomposed.m_pixelFormat);
	GLenum pixelType = static_cast<GLenum>(decomposed.m_pixelDataType);




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
				m_glTextureHandle,
				mipLevel, 0, 0,
				mipWidth, mipHeight,
				pixelFormat,
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
			//LOG_INFO("Buffer Size: "<< (width * height));
			imageData = data.m_textureData;
		}

		LOG_INFO("Uploading Texture Data: [" << width << "x" << height << "], "
				<< "internal format: [" << m_textureProperties.m_internalImageFormat << "] "
				<< "and pixel type: [" << m_textureProperties.m_pixelDataType << "]"
		);

		glTextureSubImage2D(
			m_glTextureHandle,
			0, 0, 0,
			width, height,
			pixelFormat,
			pixelType,
			imageData
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_glTextureHandle);
		}

	}
}
bool TextureGPU::NeedUpdate() const {
	return m_uploadNeedUpdate;
}
bool TextureGPU::NeedAllocate() const {
	return !m_allocated || m_reallocateDirty;
}


void TextureGPU::UploadTexture2DData(TextureProperties::TextureUploadData _imageData) const {

	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	const auto decomposed = TextureProperties::OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);

	GLenum pixelFormat = static_cast<GLenum>(decomposed.m_pixelFormat);
	GLenum pixelType = static_cast<GLenum>(decomposed.m_pixelDataType);

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
				m_glTextureHandle, 
				mipLevel, 0, 0, 
				mipWidth, mipHeight, 
				pixelFormat, 
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
			m_glTextureHandle, 0, 
			0, 0, 
			width, height, 
			pixelFormat, 
			pixelType, 
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_glTextureHandle);
		}
	}
}

void TextureGPU::UploadCubemapData(TextureProperties::TextureUploadData _imageData) const {
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
					m_glTextureHandle, mipLevel,
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
				m_glTextureHandle, 0,
				0, 0, i,
				width, height, 1,
				imageFormat,
				pixelType,
				data
			);
		}
		if (autogenMips) {
			glGenerateTextureMipmap(m_glTextureHandle);
		}
	}
}

void TextureGPU::Upload3DTextureData(TextureProperties::TextureUploadData _imageData) const {
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
				m_glTextureHandle, mipLevel,
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
			m_glTextureHandle, 0,
			0, 0, 0,
			width, height, depth,
			imageFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_glTextureHandle);
		}
	}
}

void TextureGPU::UploadCubemapArrayData(TextureProperties::TextureUploadData _imageData) const {
}

void TextureGPU::UploadTexture2DArrayData(TextureProperties::TextureUploadData _imageData) const {
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
				m_glTextureHandle, mipLevel,
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
			m_glTextureHandle, 0,
			0, 0, 0,
			width, height, layerDepth,
			imageFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(m_glTextureHandle);
		}
	}
}

TextureIDInfo& TextureIDInfo::operator=(const TextureIDInfo& _other) {
	m_textureID = _other.m_textureID;
	m_textureManager = _other.m_textureManager;
	return *this;
}

bool TextureIDInfo::IsValid() const {
	return m_textureID != C_INVALID_TEXTURE_ID && m_textureManager;
}
TextureID TextureIDInfo::GetTextureID() const { 
	return m_textureID; 
}
TextureManager* TextureIDInfo::GetTextureManager() const { 
	return m_textureManager; 
}



// -----------------------------------------------------------------


Texture::Texture(const TextureIDInfo& _info) {
	m_textureIdInfo = _info;
}


bool Texture::IsValid() const {
	return m_textureIdInfo.IsValid();
}

const TextureIDInfo& Texture::GetTextureIDInfo() const {
	return m_textureIdInfo;
} 

void Texture::SetTextureIDInfo(const TextureIDInfo& _info) {
	m_textureIdInfo = _info;
}

void Texture::SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	SparseSetView<TextureGPU> texHandle = m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID());
	if (!texHandle) return;
	TextureGPU& tex = *texHandle;
	tex.SetFilterBehaviourMin(_filterBehaviour);
}

void Texture::SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	SparseSetView<TextureGPU> texHandle = m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID()); 
	if (!texHandle) return;
	TextureGPU& tex = *texHandle;
	tex.SetFilterBehaviourMag(_filterBehaviour);
}

TextureProperties::FilterBehaviour Texture::GetMinFilter() const {
	TextureGPU& texHandle = *m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID());
	return texHandle.GetFilterBehaviourMin(); 
}

TextureProperties::FilterBehaviour Texture::GetMagFilter() const { 
	TextureGPU& texHandle = *m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID());
	return texHandle.GetFilterBehaviourMag();
}

void Texture::SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	assert(m_textureIdInfo.IsValid() && "Texture Info invalid");
	TextureGPU& texHandle = GetTextureGPU();
	texHandle.SetWrapBehaviourU(_wrapBehaviour);
}

void Texture::SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	assert(m_textureIdInfo.IsValid() && "Texture Info invalid");
	TextureGPU& texHandle = GetTextureGPU();
	texHandle.SetWrapBehaviourV(_wrapBehaviour);
}


void Texture::SetInternalFormat(const TextureProperties::TextureFormat& _format) {
	assert(m_textureIdInfo.IsValid() && "Texture Info invalid");
	TextureGPU& texHandle = GetTextureGPU();
	texHandle.SetInternalImageFormat(_format);
} 

void Texture::SetPixelColor(unsigned _col, int _x, int _y, int _z) {
	SetPixelColor(HexToVec4F(_col), _x, _y, _z);
}

void Texture::SetPixelColor(glm::vec4 _col, int _x, int _y, int _z) {
	if (!m_textureIdInfo.IsValid()) return;
	TextureGPU& tex{ GetTextureGPU() };
	glm::u8vec4 colData{};
	colData.r = _col.r * 0xff;
	colData.g = _col.g * 0xff;
	colData.b = _col.b * 0xff;
	colData.a = _col.a * 0xff;
	tex.SetPixelColor(colData, glm::ivec3{_x, _y, _z});
}

 
TextureGPU& Texture::GetTextureGPU() {
	assert(m_textureIdInfo.IsValid() && "Texture Info invalid"); 
	SparseSetView<TextureGPU> texHandle = m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID());
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

const TextureGPU& Texture::GetTextureGPU() const {
	assert(m_textureIdInfo.IsValid() && "Texture Info invalid");
	SparseSetView<TextureGPU> texHandle = m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID());
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

GLuint Texture::GetTextureHandle() const {
	if (!m_textureIdInfo.IsValid()) return 0;
	SparseSetView<TextureGPU> texHandle{ m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID()) };
	if (!texHandle) return 0;
	const TextureGPU& tex { *texHandle };
	return tex.GetTextureHandle();
}


std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureType& _type) {
	using namespace TextureProperties;
	switch (_type) {
		case TextureType::TEXTURE_1D:
			_os << "TEXTURE_1D";
			break;
		case TextureType::TEXTURE_1D_ARRAY:
			_os << "TEXTURE_1D_ARRAY";
			break;
		case TextureType::TEXTURE_2D:
			_os << "TEXTURE_2D";
			break;
		case TextureType::TEXTURE_2D_ARRAY:
			_os << "TEXTURE_2D_ARRAY";
			break;
		case TextureType::TEXTURE_3D:
			_os << "TEXTURE_3D";
			break;
		case TextureType::CUBEMAP:
			_os << "TEXTURE_CUBEMAP";
			break;
		case TextureType::CUBEMAP_ARRAY:
			_os << "CUBEMAP_ARRAY";
			break;
		default:
			_os << "UNKNOWN_TEXTURE_TYPE";
			break;
	}
	
	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageDataType& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case ImageDataType::UINT_8:
		_os << "UNSIGNED_BYTE";
		break;
	//case ImageDataType::INT_8:
		//_os << "BYTE";
		//break;
	//case PixelDataType::UNSIGNED_SHORT:
	//	_os << "UNSIGNED_SHORT";
	//	break;
	//case PixelDataType::SHORT:
	//	_os << "SHORT";
	//	break;
	case ImageDataType::FLOAT_32: 
		_os << "FLOAT";
		break;
	default:
		_os << "UNKNOWN_PIXEL_TYPE";
	}
	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageChannels& _type) {
	using namespace TextureProperties; 
	switch (_type) {
	case ImageChannels::Depth:
		_os << "DEPTH";
		break;
	case ImageChannels::Red:
		_os << "RED";
		break;
	case ImageChannels::RG:
		_os << "RG";
		break;
	case ImageChannels::RGB:
		_os << "RGB";
		break;
	case ImageChannels::RGBA:
		_os << "RGBA";
		break;
	default:
		_os << "UNKNOWN_PIXEL_FORMAT";
	}
	return _os;
}
	
std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureFormat& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case TextureFormat::R8:
		_os << "R8";
		break;
	case TextureFormat::RGB8:
		_os << "RGB8";
		break;
	case TextureFormat::RGBA8:
		_os << "RGBA8";
		break;



	case TextureFormat::SRGB8:
		_os << "SRGB8";
		break;
	case TextureFormat::SRGBA8:
		_os << "SRGBA8";
		break;



	case TextureFormat::R16F:
		_os << "R16F";
		break;
	case TextureFormat::RGB16F:
		_os << "RGB16F";
		break;
	case TextureFormat::RGBA16F:
		_os << "RGBA16F";
		break;


	case TextureFormat::DEPTH24:
		_os << "DEPTH24";
		break;
	case TextureFormat::DEPTH32F:
		_os << "DEPTH32F";
		break;


	default:
		_os << "UNKNOWN_IMAGE_FORMAT";
	}
	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::WrapBehaviour& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case WrapBehaviour::MIRROR_REPEAT:
		_os << "MIRROR_REPEAT";
		break;
	case WrapBehaviour::REPEAT:
		_os << "REPEAT";
		break;
	case WrapBehaviour::TO_BORDER:
		_os << "TO_BORDER";
		break;
	case WrapBehaviour::TO_EDGE:
		_os << "TO_EDGE";
		break;
	default:
		_os << "_UNDEFINED_WRAP_BEHAVIOUR";
	}
	return _os;

}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::FilterBehaviour& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case FilterBehaviour::LINEAR:
		_os << "LINEAR";
		break;
	case FilterBehaviour::NEAREST:
		_os << "NEAREST";
		break;
	default:
		_os << "_UNDEFINED_FILTER_BEHAVIOUR";
	}
	return _os;
}
   