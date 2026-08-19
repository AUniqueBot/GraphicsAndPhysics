#include <arch/resources/res_gpu_resources/res_gpu_texture.h>


GPU_Texture::GPU_Texture(
	TextureProperties::TextureType _type,
	glm::ivec3 _dims,
	TextureProperties::TextureProps _props
) {
	using namespace TextureProperties;
	m_textureType = _type;
	m_dimensions = _dims;
	m_textureProperties = _props;

}



GPU_Texture::~GPU_Texture() {
	if (!m_handle.IsValid()) return;
	LOG_INFO("Destroying Texture with handle [" << m_handle << "]");
	glDeleteTextures(1, &m_handle.Get());
}

GPU_Texture::GPU_Texture(GPU_Texture&& _old) noexcept {
	m_textureType = _old.m_textureType;
	m_handle = _old.m_handle;
	m_dimensions = _old.m_dimensions;
	m_textureProperties = _old.m_textureProperties;

	m_allocated = _old.m_allocated;
	m_uploaded = _old.m_uploaded;
	m_uploadNeedUpdate = _old.m_uploadNeedUpdate;
	m_reallocateDirty = _old.m_reallocateDirty;
	m_samplingDirty = _old.m_samplingDirty;



	_old.m_handle.Get() = 0;
}

GPU_Texture& GPU_Texture::operator=(GPU_Texture&& _old) noexcept {
	m_textureType = _old.m_textureType;
	m_dimensions = _old.m_dimensions;
	m_textureProperties = _old.m_textureProperties;
	m_handle = _old.m_handle;


	m_allocated = _old.m_allocated;
	m_uploaded = _old.m_uploaded;
	m_uploadNeedUpdate = _old.m_uploadNeedUpdate;
	m_reallocateDirty = _old.m_reallocateDirty;
	m_samplingDirty = _old.m_samplingDirty;


	// ensure this one doesn't get it.
	_old.m_handle.Get() = 0;
	return *this;
}

void GPU_Texture::Load(const TextureRes& _tex) {

}

const GLuint& GPU_Texture::GetTextureHandle() const {
	return m_handle.Get();
}

const TextureProperties::TextureType& GPU_Texture::GetTextureType() const {
	return m_textureType;
}

const TextureProperties::TextureFormat& GPU_Texture::GetInternalImageFormat() const {
	return m_textureProperties.m_internalImageFormat;
}

void GPU_Texture::SetInternalImageFormat(const TextureProperties::TextureFormat& _format) {
	if (_format == m_textureProperties.m_internalImageFormat) return;
	m_textureProperties.m_internalImageFormat = _format;
	m_reallocateDirty = true;
}

const TextureProperties::ImageDataType& GPU_Texture::GetDataType() const {
	return m_textureProperties.m_pixelDatatype;
}



void GPU_Texture::SetDataType(const TextureProperties::ImageDataType& _pixelDataType) {
	if (_pixelDataType == m_textureProperties.m_pixelDatatype) return;
	m_textureProperties.m_pixelDatatype = _pixelDataType;
	m_reallocateDirty = true;
}

const TextureProperties::ImageChannels& GPU_Texture::GetChannels() const {
	return m_textureProperties.m_pixelFormat;
}

void GPU_Texture::SetChannels(const TextureProperties::ImageChannels& _pixelFormat) {
	if (_pixelFormat == m_textureProperties.m_pixelFormat) return;
	m_textureProperties.m_pixelFormat = _pixelFormat;
	m_reallocateDirty = true;
}

void GPU_Texture::SetWrapBehaviourU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	if (m_textureProperties.m_wrapU == _wrapBehaviour) return;
	m_textureProperties.m_wrapU = _wrapBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::WrapBehaviour& GPU_Texture::GetWrapBehaviourU() const {
	return m_textureProperties.m_wrapU;
}

void GPU_Texture::SetWrapBehaviourV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	if (m_textureProperties.m_wrapV == _wrapBehaviour) return;
	m_textureProperties.m_wrapV = _wrapBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::WrapBehaviour& GPU_Texture::GetWrapBehaviourV() const {
	return m_textureProperties.m_wrapV;
}

void GPU_Texture::SetFilterBehaviourMin(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	if (m_textureProperties.m_filterMin == _filterBehaviour) return;
	m_textureProperties.m_filterMin = _filterBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::FilterBehaviour& GPU_Texture::GetFilterBehaviourMin() const {
	return m_textureProperties.m_filterMin;
}

void GPU_Texture::SetFilterBehaviourMag(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	if (m_textureProperties.m_filterMag == _filterBehaviour) return;
	m_textureProperties.m_filterMag = _filterBehaviour;
	m_samplingDirty = true;
}

const TextureProperties::FilterBehaviour& GPU_Texture::GetFilterBehaviourMag() const {
	return m_textureProperties.m_filterMag;
}

void GPU_Texture::SetDimensions(glm::ivec3 _dims) {
	assert(_dims.x > 0 && _dims.y > 0 && _dims.z > 0 && "attempting to set invalid dimensions");
	if (m_dimensions == _dims) return;
	m_dimensions = _dims;
	m_reallocateDirty = true;
}

const glm::ivec3& GPU_Texture::GetDimensions() const {
	return m_dimensions;
}

const int& GPU_Texture::GetX() const {
	return m_dimensions.x;
}

const int& GPU_Texture::GetY() const {
	return m_dimensions.y;
}

const int& GPU_Texture::GetZ() const {
	return m_dimensions.z;
}

void GPU_Texture::SetX(const int& _val) {
	assert(_val > 0 && "attempting to set to invalid size");
	if (_val == m_dimensions.x) return;
	m_dimensions.x = _val;
	m_reallocateDirty = true;
}

void GPU_Texture::SetY(const int& _val) {
	assert(_val > 0 && "attempting to set to invalid size");
	if (_val == m_dimensions.y) return;
	m_dimensions.y = _val;
	m_reallocateDirty = true;
}

void GPU_Texture::SetZ(const int& _val) {
	assert(_val > 0 && "attempting to set to invalid size");
	if (_val == m_dimensions.z) return;
	m_dimensions.z = _val;
	m_reallocateDirty = true;
}

void GPU_Texture::SetPixelColor(glm::u8vec1 _col, glm::ivec3 _pixelPos) {
	if (!m_allocated) return;
	int x = _pixelPos.x, y = _pixelPos.y, z = _pixelPos.z;
	using namespace TextureProperties;
	InternalImageDecomposed decomposed = OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);// this controls
	int pixelFormat = static_cast<GLenum>(decomposed.m_pixelFormat);
	int pixelType = static_cast<GLenum>(decomposed.m_pixelDatatype);
	int uploadDimCount = GetUploadDimension(m_textureType);
}

void GPU_Texture::SetPixelColor(glm::u8vec4 _col, glm::ivec3 _pixelPos) {
	if (!m_allocated) return;
	int x = _pixelPos.x, y = _pixelPos.y, z = _pixelPos.z;
	using namespace TextureProperties;
	InternalImageDecomposed decomposed = OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);
	int pixelFormat = OpenGL::ResolveChannels(decomposed.m_pixelFormat);
	int pixelType = OpenGL::ResolveDataType(decomposed.m_pixelDatatype);
	int uploadDimCount = GetUploadDimension(m_textureType);

	glm::u8vec4 u8vec4col = {};
	glm::vec4 vec4col = {};
	GLuint handle = m_handle.Get();
	switch (uploadDimCount) {
	case 1:
		assert(x < m_dimensions.x && x >= 0);
		glTextureSubImage1D(
			handle, 0, x, 1,
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
			handle, 0, x, y, 1, 1,
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

void GPU_Texture::UpdateTextureProperties() {
	if (!m_samplingDirty) return;

	GLuint handle = m_handle.Get();
	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_textureProperties.m_filterMag));
	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_textureProperties.m_filterMin));
	glTextureParameteri(handle, GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_textureProperties.m_wrapU));
	glTextureParameteri(handle, GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_textureProperties.m_wrapV));
	m_samplingDirty = false;
}

void GPU_Texture::UpdateAllocation() {
	if (m_allocated && !m_reallocateDirty) return;
	Destroy();
	Create();
	Allocate();
}

void GPU_Texture::Create() {
	if (m_handle.IsValid()) return;
	glCreateTextures(static_cast<GLenum>(m_textureType), 1, &m_handle.Get());
	assert(m_handle.IsValid() && "Failed to create texture.");
}

void GPU_Texture::Destroy() {
	if (!m_handle.IsValid()) return;
	glDeleteTextures(1, &m_handle.Get());
	m_handle.Get() = 0;
	m_allocated = false;
	m_uploaded = false;
}

void GPU_Texture::Allocate() {
	using namespace TextureProperties;
	LOG_DEBUG("Allocating for texture handle [" << m_handle.Get() << "] as " << m_textureType);
	int uploadDimensionCount = GetUploadDimension(m_textureType);

	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int depth = m_dimensions.z;
	GLenum internalFormat = OpenGL_ImageFormatToEnum(m_textureProperties.m_internalImageFormat);
	int mipCount = m_textureProperties.m_mipmapCount > 0 ? m_textureProperties.m_mipmapCount : 1;
	GLuint id = m_handle.Get();
	switch (uploadDimensionCount) {
	case 1:
		glTextureStorage1D(id, mipCount, internalFormat, width);
		break;
	case 2:
		glTextureStorage2D(id, mipCount, internalFormat, width, height);
		break;
	case 3:
		glTextureStorage3D(id, mipCount, internalFormat, width, height, depth);
		break;
	}
	m_allocated = true;
}


void GPU_Texture::Upload(TextureProperties::TextureUploadData _imageData) const {
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
	GLenum pixelType = static_cast<GLenum>(decomposed.m_pixelDatatype);


	GLuint handle = m_handle.Get();

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
				handle,
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
			<< "and pixel type: [" << m_textureProperties.m_pixelDatatype << "]"
		);

		glTextureSubImage2D(
			handle,
			0, 0, 0,
			width, height,
			pixelFormat,
			pixelType,
			imageData
		);
		if (autogenMips) {
			glGenerateTextureMipmap(handle);
		}

	}
}
bool GPU_Texture::NeedUpdate() const {
	return m_uploadNeedUpdate;
}
bool GPU_Texture::NeedAllocate() const {
	return !m_allocated || m_reallocateDirty;
}


void GPU_Texture::UploadTexture2DData(TextureProperties::TextureUploadData _imageData) const {

	GLuint handle = m_handle.Get();
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	const auto decomposed = TextureProperties::OpenGL_ToDecomposed(m_textureProperties.m_internalImageFormat);

	GLenum pixelFormat = static_cast<GLenum>(decomposed.m_pixelFormat);
	GLenum pixelType = static_cast<GLenum>(decomposed.m_pixelDatatype);

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
				handle,
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
			handle, 0,
			0, 0,
			width, height,
			pixelFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(handle);
		}
	}
}

void GPU_Texture::UploadCubemapData(TextureProperties::TextureUploadData _imageData) const {

	GLuint handle = m_handle.Get();
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDatatype);



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
					handle, mipLevel,
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
				handle, 0,
				0, 0, i,
				width, height, 1,
				imageFormat,
				pixelType,
				data
			);
		}
		if (autogenMips) {
			glGenerateTextureMipmap(handle);
		}
	}
}

void GPU_Texture::Upload3DTextureData(TextureProperties::TextureUploadData _imageData) const {

	GLuint handle = m_handle.Get();
	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int depth = m_dimensions.z;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDatatype);

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
				handle, mipLevel,
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
			handle, 0,
			0, 0, 0,
			width, height, depth,
			imageFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(handle);
		}
	}
}

void GPU_Texture::UploadCubemapArrayData(TextureProperties::TextureUploadData _imageData) const {
}

void GPU_Texture::UploadTexture2DArrayData(TextureProperties::TextureUploadData _imageData) const {

	GLuint handle = m_handle.Get();

	int width = m_dimensions.x;
	int height = m_dimensions.y;
	int layerCount = m_dimensions.z;
	int mipCount = m_textureProperties.m_mipmapCount;
	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
	bool hasExternalData = _imageData != std::nullopt;
	int layerDepth = 1;

	GLenum imageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDatatype);

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
				handle, mipLevel,
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
			handle, 0,
			0, 0, 0,
			width, height, layerDepth,
			imageFormat,
			pixelType,
			nullptr
		);
		if (autogenMips) {
			glGenerateTextureMipmap(handle);
		}
	}
}
