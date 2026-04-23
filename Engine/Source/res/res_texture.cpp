#include <pch.h>
#include <arch/resources/res_texture.h>
#include <stb_image.h>


namespace TextureCreation {
	Texture CreateTexture(
		const TextureCreationInfo& _info,
		const void* _data
	) {
		Texture tex{};
		GLuint texId{};

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		GLenum internalFormat		{ static_cast<GLenum>(_info.m_internalFormat) };
		GLenum pixelFormat			{ static_cast<GLenum>(_info.m_format) };
		GLenum pixelType			{ static_cast<GLenum>(_info.m_type) };
		

		int width{ _info.m_resolution.x  };
		int height{ _info.m_resolution.y };
		int mipLevels				{ _info.m_mipLevels };
		bool autoGenerateMips		{ mipLevels == 0 };
		if (autoGenerateMips)		mipLevels = 1;

		for (int mipLevel{}; mipLevel < mipLevels; ++mipLevel) {
			int width	{ _info.m_resolution.x >> mipLevel };
			int height	{ _info.m_resolution.y >> mipLevel };
			glTexImage2D(GL_TEXTURE_2D, mipLevel,
				internalFormat,
				width, height,
				0,
				pixelFormat,
				pixelType,
				_data
			);
		}
		if (autoGenerateMips) {
			glGenerateTextureMipmap(GL_TEXTURE_2D);
			mipLevels = std::floor(std::log2(std::max(width,height))) + 1;
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		// set alloc'd params.
		tex.SetTextureID(texId);
		tex.SetMipmapCount(mipLevels);
		tex.SetDimensions({width, height});


		return tex;
	}

	// - image loading ----------------------------------------------
	static Texture _LoadImageFromFile(
		const std::filesystem::path& path,
		PixelDataType _pixType
	) {
		TextureCreationInfo texInfo{};
		int channels{};


		using ImageLoadFunction = void* (*)(const char* _filePath, int* x, int* y, int* _channelsInFile, int _desiredChannels);
		ImageLoadFunction fn { nullptr };
		switch (_pixType) {
		case PixelDataType::UNSIGNED_BYTE:
		case PixelDataType::BYTE:
			fn = (ImageLoadFunction)stbi_load;
			break;
		case PixelDataType::UNSIGNED_SHORT:
		case PixelDataType::SHORT:
			fn = (ImageLoadFunction)stbi_load_16;
			break;
		case PixelDataType::FLOAT:
			fn = (ImageLoadFunction)stbi_loadf;
			break;
		}
		if (fn == nullptr) {
			throw std::runtime_error("No function provided for this");
		}
		
		void* imgData { fn(path.string().c_str(), &texInfo.m_resolution.x, &texInfo.m_resolution.y, &channels, 0) };
		texInfo.m_type = _pixType;

		Texture tex{ CreateTexture(texInfo, imgData) };
		stbi_image_free(imgData);

		return tex;
	}


	Texture LoadTextureFromFile(
		const std::filesystem::path& path,
		ImagePrecision _precision
	) {
		std::function<Texture(const std::filesystem::path&)> fn;
		switch (_precision) {
		case ImagePrecision::UINT8:
			fn = [](const std::filesystem::path& _path) { return _LoadImageFromFile(_path, PixelDataType::UNSIGNED_BYTE); };
			break;
		case ImagePrecision::UINT16:
			fn = [](const std::filesystem::path& _path) { return _LoadImageFromFile(_path, PixelDataType::UNSIGNED_SHORT); };
			break;
		case ImagePrecision::FLOAT32:
			fn = [](const std::filesystem::path& _path) { return _LoadImageFromFile(_path, PixelDataType::FLOAT); };
			break;
		}
		return fn(path);
	}
}





bool Texture::BindTexture() const {
	if (m_textureID != 0) glBindTexture(GL_TEXTURE_2D, m_textureID);
	return m_textureID != 0;
}

void Texture::SetWrappingBehavior(WrapBehaviour _u, WrapBehaviour _v) {
	SetWrappingBehaviorU(_u);
	SetWrappingBehaviorV(_v);

}

void Texture::SetWrappingBehaviorU(WrapBehaviour _setting) {
	m_wrapU = _setting;
}

void Texture::SetWrappingBehaviorV(WrapBehaviour _setting) {
	m_wrapV = _setting;
}

std::pair<
	Texture::WrapBehaviour,
	Texture::WrapBehaviour
> Texture::GetWrappingBehaviour() const {
	return { GetWrappingBehaviourU(), GetWrappingBehaviourV() };
}

const Texture::WrapBehaviour& Texture::GetWrappingBehaviourU() const {
	return m_wrapU;
}

const Texture::WrapBehaviour& Texture::GetWrappingBehaviourV() const {
	return m_wrapV;
}



void Texture::SetFilterBehavior(FilterBehaviour _h, FilterBehaviour _v) {
	SetFilterBehaviorMin(_h);
	SetFilterBehaviorMag(_v);
}

void Texture::SetFilterBehaviorMin(FilterBehaviour _setting) {
	m_filterMin = _setting;
}

void Texture::SetFilterBehaviorMag(FilterBehaviour _setting) {
	m_filterMag = _setting;
}

const TextureCreation::PixelFormat& Texture::GetPixelFormat() const {
	return m_pixelFormat;
}

const TextureCreation::PixelDataType& Texture::GetPixelDataType() const {
	return m_pixelDataType;
}

const TextureCreation::InternalImageFormat& Texture::GetInternalImageFormat() const {
	return m_iImageFormat;
}




std::pair<
	Texture::FilterBehaviour, 
	Texture::FilterBehaviour
> Texture::GetFilterBehaviour() const {
	return { GetFilterBehaviourMin(), GetFilterBehaviourMag() };
}

const Texture::FilterBehaviour& Texture::GetFilterBehaviourMin() const {
	return m_filterMin;
}

const Texture::FilterBehaviour& Texture::GetFilterBehaviourMag() const {
	return m_filterMag;
}

void Texture::SetMipmapCount(int _setting) {
	m_mipmapCount = _setting;
}

int Texture::GetMipCount() const {
	return m_mipmapCount;
}

void Texture::SetTextureID(const Texture::TextureID& _id) {
	m_textureID = _id;
}

void Texture::SetDimensions(const glm::ivec2& _dimensions) {
	m_dimensions = _dimensions;
}

const Texture::TextureID& Texture::GetTextureID() const {
	return m_textureID;
}

void Texture::UpdateResourceParameters() {
	if (!BindTexture()) return;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_wrapU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_wrapV));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_filterMin));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_filterMag));
}


