#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


class Texture;
namespace TextureCreation {

	enum class PixelDataType : GLenum {
		UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		BYTE = GL_BYTE,
		UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
		SHORT = GL_SHORT,
		FLOAT = GL_FLOAT
	};

	enum class PixelFormat : GLenum {
		RED = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH = GL_DEPTH_COMPONENT
	};

	enum class InternalImageFormat : GLenum {
		R8 = GL_R8,
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8,

		SRGB8 = GL_SRGB8,
		SRGBA8 = GL_SRGB8_ALPHA8,

		R16F = GL_R16F,
		RGB16F = GL_RGB16F,
		RGBA16F = GL_RGBA16F,

		DEPTH24 = GL_DEPTH_COMPONENT24,
		DEPTH32F = GL_DEPTH_COMPONENT32F
	};


	struct TextureCreationInfo {
		glm::ivec2 m_resolution = { 1, 1 };

		InternalImageFormat m_internalFormat = InternalImageFormat::RGBA8;   // GPU storage format
		PixelFormat m_format = PixelFormat::RGBA;    // input data format
		PixelDataType m_type = PixelDataType::UNSIGNED_BYTE;

		int m_mipLevels = 1; // 0 = autogenerate. N > 0 = specified number of mips.
	};



	Texture CreateTexture(
		const TextureCreationInfo& _info,
		const void* _data = nullptr
	);

	// - load from file -----------------------------------------------

	enum class ImagePrecision {
		UINT8,
		UINT16,
		FLOAT32
	};

	// uint8 is the default image precision. override this if you are loading images such as hdris.
	Texture LoadTextureFromFile(
		const std::filesystem::path& _path,
		ImagePrecision _precision = ImagePrecision::UINT8 
	);
}


namespace TextureSettings {
	enum class WrapBehaviour : int {
		REPEAT = GL_REPEAT,
		MIRROR_REPEAT = GL_MIRRORED_REPEAT,
		TO_EDGE = GL_CLAMP_TO_EDGE,
		TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum class FilterBehaviour : int {
		LINEAR = GL_LINEAR,
		NEAREST = GL_NEAREST
	};

}


class Texture : public Resource<Texture> {
private:
public:
	using PixelFormat = TextureCreation::PixelFormat;
	using PixelDataType = TextureCreation::PixelDataType;
	using InternalImageFormat = TextureCreation::InternalImageFormat;

	using WrapBehaviour = TextureSettings::WrapBehaviour;
	using FilterBehaviour = TextureSettings::FilterBehaviour;

	using TextureID = unsigned;
public:
	// constructors
	Texture() = default;
	Texture(Texture& _texture) = delete;
	Texture(const Texture& _texture) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& _texture) noexcept;

	~Texture();


public:
	// either path or image.


	// - settings -------------------
	void SetWrappingBehavior(WrapBehaviour _u, WrapBehaviour _v);
	void SetWrappingBehaviorU(WrapBehaviour _setting);
	void SetWrappingBehaviorV(WrapBehaviour _setting);
	std::pair<WrapBehaviour, WrapBehaviour> GetWrappingBehaviour() const;
	const WrapBehaviour& GetWrappingBehaviourU() const;
	const WrapBehaviour& GetWrappingBehaviourV() const;



	void SetFilterBehavior(FilterBehaviour _h, FilterBehaviour _v);
	void SetFilterBehaviorMin(FilterBehaviour _setting);
	void SetFilterBehaviorMag(FilterBehaviour _setting);
	std::pair<FilterBehaviour, FilterBehaviour> GetFilterBehaviour() const;
	const FilterBehaviour& GetFilterBehaviourMin() const;
	const FilterBehaviour& GetFilterBehaviourMag() const;


	// - readonly parameters -------
	int GetMipCount() const;
	const glm::ivec2& GetDimensions() const;
	const TextureID& GetTextureID() const;

	const PixelFormat& GetPixelFormat() const;
	const PixelDataType& GetPixelDataType() const;
	const InternalImageFormat& GetInternalImageFormat() const;

	
private:
	// - friend function -----------
	friend Texture TextureCreation::CreateTexture(const TextureCreationInfo& _info, const void* _data);
	void SetTextureID(const TextureID& _textureId); 
	void SetDimensions(const glm::ivec2& _dimensions);
	void SetMipmapCount(int _setting);


private:
	// to be removed.
	bool BindTexture() const;
	void UpdateResourceParameters();

private:
	void Destroy();


private:
	// - readonly parameters -------------------------------------
	TextureID m_textureID					{ 0 };
	glm::ivec2 m_dimensions					{ 0, 0 };	// automatically read on loading of image.	
	int m_mipmapCount						{ 1 };		// default 1, no mips created.

	PixelFormat m_pixelFormat				{ PixelFormat::RGBA };
	PixelDataType m_pixelDataType			{ PixelDataType::UNSIGNED_BYTE };
	InternalImageFormat m_iImageFormat		{ InternalImageFormat::RGBA8 };
	
	// - sampling parameters -------------------------------------
	WrapBehaviour m_wrapU					{ WrapBehaviour::REPEAT };
	WrapBehaviour m_wrapV					{ WrapBehaviour::REPEAT };
	
	FilterBehaviour m_filterMin				{ FilterBehaviour::LINEAR };
	FilterBehaviour m_filterMag				{ FilterBehaviour::LINEAR };
	


};


