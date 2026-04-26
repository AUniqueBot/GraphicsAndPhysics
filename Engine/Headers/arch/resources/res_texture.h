#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


class Texture;
namespace TextureProperties {

	enum class TextureType : GLenum {

		TEXTURE_1D = GL_TEXTURE_1D,
		TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY, 
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_3D = GL_TEXTURE_3D,
		CUBEMAP = GL_TEXTURE_CUBE_MAP,
		CUBEMAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
	};


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


	// generating textures.
	struct SubImageData {
		int m_layer								{};
		int m_mipLevel							{};
		const void* m_data;
	};

	struct TextureUploadInfo {
		int m_mipLevels							{ 1 };
		int m_textureDimensionCount				{ 2 }; // 1, 2, 3.
		int m_layerCount						{ 0 }; // == 1 -> non-array;  > 1 -> array
		int m_faces								{ 1 }; // 1, 6
		std::vector<SubImageData> m_imageData;
	};

	struct TextureCreationInfo {

		// size
		TextureType m_textureType = TextureType::TEXTURE_2D;
		glm::ivec3 m_resolution = { 1, 1, 1 };

		InternalImageFormat m_internalImageFormat = InternalImageFormat::RGBA8;   // GPU storage format
		PixelFormat m_pixelFormat = PixelFormat::RGBA;    // input data format
		PixelDataType m_pixelDataType = PixelDataType::UNSIGNED_BYTE;
		int m_mipLevels = 1; // 0 = autogenerate. N > 0 = specified number of mips.
		std::optional<TextureUploadInfo> m_textureData;
	};




}

namespace TextureCreation {

	Texture CreateTexture(
		const TextureProperties::TextureCreationInfo& _info,
		std::optional<TextureProperties::TextureUploadInfo> _imageData = std::nullopt
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



class Texture : public Resource<Texture> {
private:
public:
	using TextureType = TextureProperties::TextureType;
	using PixelFormat = TextureProperties::PixelFormat;
	using PixelDataType = TextureProperties::PixelDataType;
	using InternalImageFormat = TextureProperties::InternalImageFormat;
	using TextureCreationInfo = TextureProperties::TextureCreationInfo;
	using WrapBehaviour = TextureProperties::WrapBehaviour;
	using FilterBehaviour = TextureProperties::FilterBehaviour;

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
	const glm::ivec3& GetDimensions() const;
	const TextureID& GetTextureID() const;
	const bool& GetResizeAllowed() const;


	const PixelFormat& GetPixelFormat() const;
	const PixelDataType& GetPixelDataType() const;
	const InternalImageFormat& GetInternalImageFormat() const;

public:
	void Resize(const glm::ivec3& _dimensions);
	bool IsValid() const;
	bool IsArray() const;
	int DimensionCount() const;
public:
	void SetTextureID(const TextureID& _textureId); 
	void SetTextureType(const TextureType& _type);
	void SetDimensions(const glm::ivec3& _dimensions);
	void SetMipmapCount(int _setting);
	void SetResizeAllowed(bool _setting);
	void SetPixelFormat(const PixelFormat& _format);
	void SetPixelDataType(const PixelDataType& _format);
	void SetInternalImageFormat(const InternalImageFormat& _format);



	// to be removed.
	bool BindTexture() const;
	void UpdateResourceParameters();

private:
	friend Texture TextureCreation::CreateTexture(
		const TextureCreationInfo& _info, 
		std::optional<TextureProperties::TextureUploadInfo> _imageData
	);
	TextureProperties::TextureCreationInfo GetTextureCreationInfo() const;

	void Build(
		const TextureProperties::TextureCreationInfo& _info,
		std::optional<TextureProperties::TextureUploadInfo> _imageData = std::nullopt
	);

	void GenerateTexture(std::optional<TextureProperties::TextureUploadInfo> _imageData = std::nullopt);

	void Destroy();


private:
	// - rebuild parameters ---------------------------------------
	TextureID m_textureID					{ 0 };

	TextureType m_textureType				{ TextureType::TEXTURE_2D };
	glm::ivec3 m_dimensions					{ 0, 0, 0 };	// automatically read on loading of image.	
	int m_mipmapCount						{ 1 };		// default 1, no mips created.
	std::optional<int> m_layerCount; 
	
	bool m_resizeAllowed					{ true };
	bool m_autoGenerateMips					{ false };

	PixelFormat m_pixelFormat				{ PixelFormat::RGBA };
	PixelDataType m_pixelDataType			{ PixelDataType::UNSIGNED_BYTE };
	InternalImageFormat m_iImageFormat		{ InternalImageFormat::RGBA8 };
	

	// - sampling parameters -------------------------------------
	WrapBehaviour m_wrapU					{ WrapBehaviour::REPEAT };
	WrapBehaviour m_wrapV					{ WrapBehaviour::REPEAT };
	
	FilterBehaviour m_filterMin				{ FilterBehaviour::LINEAR };
	FilterBehaviour m_filterMag				{ FilterBehaviour::LINEAR };
	


};


