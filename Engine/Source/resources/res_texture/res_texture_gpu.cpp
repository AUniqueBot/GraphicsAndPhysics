#include <arch/resources/res_texture/res_texturegpu.h>


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
