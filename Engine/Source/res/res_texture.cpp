#include <pch.h>
#include <arch/resources/res_texture.h>
#include <stb_image.h>


namespace TextureCreation {
	using namespace TextureProperties;

	Texture CreateTexture(
		const TextureCreationInfo& _info,
		std::optional<TextureProperties::TextureUploadInfo> _imageData
	) {
		Texture tex{};
		tex.Build(_info, _imageData);
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
		texInfo.m_pixelDataType = _pixType;

		TextureProperties::TextureUploadInfo info;
		
		
		info.m_faces = 1;
		info.m_layerCount = 1;
		info.m_mipLevels = 1;
		info.m_textureDimensionCount = 2;
		info.m_imageData = {};
		info.m_imageData.push_back({1, 1, imgData});


		Texture tex{ CreateTexture(texInfo) };
		stbi_image_free(imgData);

		return tex;
	}


	Texture LoadTextureFromFile(
		const std::filesystem::path& path,
		ImagePrecision _precision
	) {
		std::function<Texture(const std::filesystem::path&)> fn;
		PixelDataType type{ PixelDataType::UNSIGNED_BYTE };
		switch (_precision) {
		case ImagePrecision::UINT8:
			type = PixelDataType::UNSIGNED_BYTE;
			break;
		case ImagePrecision::UINT16:
			type = PixelDataType::UNSIGNED_SHORT;
			break;
		case ImagePrecision::FLOAT32:
			type = PixelDataType::FLOAT;
			break;
		}

		return _LoadImageFromFile(path, type);
	}
}





bool Texture::BindTexture() const {
	if (m_textureID != 0) glBindTexture(GL_TEXTURE_2D, m_textureID);
	return m_textureID != 0;
}

Texture::Texture(Texture&& _texture) noexcept {
	m_textureID = _texture.m_textureID;
	m_dimensions = _texture.m_dimensions;
	m_mipmapCount = _texture.m_mipmapCount;

	m_pixelFormat = _texture.m_pixelFormat;
	m_pixelDataType = _texture.m_pixelDataType;
	m_iImageFormat = _texture.m_iImageFormat;

	m_wrapU = _texture.m_wrapU;
	m_wrapV = _texture.m_wrapV;

	m_filterMin = _texture.m_filterMin;
	m_filterMag = _texture.m_filterMag;

	m_resizeAllowed = _texture.m_resizeAllowed;

	// you are required to call generate texture if you want to actually use it.
	// mandated.
}

Texture::~Texture() {
	Destroy();
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

const TextureProperties::PixelFormat& Texture::GetPixelFormat() const {
	return m_pixelFormat;
}

const TextureProperties::PixelDataType& Texture::GetPixelDataType() const {
	return m_pixelDataType;
}

const TextureProperties::InternalImageFormat& Texture::GetInternalImageFormat() const {
	return m_iImageFormat;
}

void Texture::Build(
	const TextureProperties::TextureCreationInfo& _info,
	std::optional<TextureProperties::TextureUploadInfo> _imageData
) {

	SetTextureType(_info.m_textureType);
	SetMipmapCount(_info.m_mipLevels);
	SetDimensions(_info.m_resolution);
	SetPixelFormat(_info.m_pixelFormat);
	SetPixelDataType(_info.m_pixelDataType);
	SetInternalImageFormat(_info.m_internalImageFormat);
	SetResizeAllowed(!_imageData.has_value());

	GenerateTexture(_imageData);
}

void Texture::GenerateTexture(std::optional<TextureProperties::TextureUploadInfo> _imageData) {
	Destroy();
	
	GLenum texType			{ static_cast<GLenum>(m_textureType) };

	// useful for 2D 

	GLenum internalFormat	{ static_cast<GLenum>(m_iImageFormat) };
	GLenum pixelFormat		{ static_cast<GLenum>(m_pixelFormat) };
	GLenum pixelType		{ static_cast<GLenum>(m_pixelDataType) };
	int width				{ m_dimensions.x };
	int height				{ m_dimensions.y };
	int depth				{ m_dimensions.z };	// treated as depth or index value for arrays.
	int mipLevels			{ m_mipmapCount };

	glGenTextures(1, &m_textureID);
	glBindTexture(texType, m_textureID);
	
	// texture types
	// resolution

	// mips
	bool autoGenerateMips{ mipLevels == 0 };
	if (autoGenerateMips)		mipLevels = 1;

	// array
	bool isArray{};
	int imageDimCount{ 0 };
	int layers{0}; // only for array type stuff.

	// allocation
	switch (m_textureType) {
	case TextureType::TEXTURE_1D:
		glTexStorage1D(texType, mipLevels, internalFormat, width);
		imageDimCount = 1;
		break;
	case TextureType::TEXTURE_1D_ARRAY:
		glTexStorage2D(texType, mipLevels, internalFormat, width, height);
		imageDimCount = 1;
		layers = height;
		isArray = true;
		break;
	case TextureType::TEXTURE_2D:
		glTexStorage2D(texType, mipLevels, internalFormat, width, height);
		imageDimCount = 2;
		break;
	case TextureType::TEXTURE_2D_ARRAY:
		glTexStorage3D(texType, mipLevels, internalFormat, width, height, depth);
		imageDimCount = 2;
		layers = depth;
		isArray = true;
		break;
	case TextureType::TEXTURE_3D:
		glTexStorage3D(texType, mipLevels, internalFormat, width, height, depth);
		imageDimCount = 3;
		break;
	case TextureType::CUBEMAP:
		glTexStorage2D(texType, mipLevels, internalFormat, width, height);
		imageDimCount = 2;
		break;
	case TextureType::CUBEMAP_ARRAY:
		glTexStorage3D(texType, mipLevels, internalFormat, width, height, 6 * depth);
		imageDimCount = 2;
		layers = depth;
		isArray = true;
		break;
	}


	if (autoGenerateMips) {
		glGenerateTextureMipmap(texType);
		mipLevels = std::floor(std::log2(std::max(width, height))) + 1;
		// update the mip count automatically.
		SetMipmapCount(mipLevels);
	}
	glBindTexture(texType, 0);
	SetTextureID(m_textureID);
}


void Texture::Resize(const glm::ivec3& _dimensions) {
	if (!GetResizeAllowed()) return;
	if (m_dimensions == _dimensions) return;
	m_dimensions = _dimensions;
	Build(GetTextureCreationInfo());
}

bool Texture::IsArray() const {
	return 
		m_textureType == TextureType::CUBEMAP_ARRAY ||
		m_textureType == TextureType::TEXTURE_1D_ARRAY||
		m_textureType == TextureType::TEXTURE_2D_ARRAY		
		;
}

int Texture::DimensionCount() const {
	return 
		(m_textureType == TextureType::TEXTURE_1D || m_textureType == TextureType::TEXTURE_1D_ARRAY) ? 1 : 
		(m_textureType == TextureType::TEXTURE_3D) ? 3 : 
		2;
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

void Texture::SetResizeAllowed(bool _setting) {
	m_resizeAllowed = _setting;
}

void Texture::SetPixelFormat(const PixelFormat& _format) {
	m_pixelFormat = _format;
}

void Texture::SetPixelDataType(const PixelDataType& _pixelDataType) {
	m_pixelDataType = _pixelDataType;
}

void Texture::SetInternalImageFormat(const InternalImageFormat& _format) {
	m_iImageFormat = _format;
}

int Texture::GetMipCount() const {
	return m_mipmapCount;
}

void Texture::SetTextureID(const Texture::TextureID& _id) {
	m_textureID = _id;
}

void Texture::SetTextureType(const TextureType& _type) {
	m_textureType = _type;
}

void Texture::SetDimensions(const glm::ivec3& _dimensions) {
	m_dimensions = _dimensions;
}

const Texture::TextureID& Texture::GetTextureID() const {
	return m_textureID;
}

const bool& Texture::GetResizeAllowed() const {
	return m_resizeAllowed;
}

void Texture::UpdateResourceParameters() {
	if (!BindTexture()) return;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_wrapU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_wrapV));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_filterMin));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_filterMag));
}


TextureProperties::TextureCreationInfo Texture::GetTextureCreationInfo() const {
	TextureProperties::TextureCreationInfo info{};

	info.m_pixelFormat			= m_pixelFormat;
	info.m_pixelDataType		= m_pixelDataType;
	info.m_internalImageFormat	= m_iImageFormat;
	
	info.m_resolution			= m_dimensions;
	info.m_mipLevels			= m_mipmapCount;

	return info;
}

void Texture::Destroy() {
	if (m_textureID == 0) return;
	glDeleteTextures(1, &m_textureID);
	m_textureID = 0;
}



