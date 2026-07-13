#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


class Texture;
using TextureID = uint16_t;
const TextureID C_INVALID_TEXTURE_ID = 0;

class TextureManager;	// fwd declaration

struct TextureIDInfo {
	// this struct is created to package the identity and source together so accessing the correct data is correct.
	// it is not responsible for the texture lifetime or how to interpret the texture data. It is simply an ID package to the texture.
	friend class TextureManager;
private:
	TextureID m_textureID				{ C_INVALID_TEXTURE_ID };
	TextureManager* m_textureManager	{ nullptr };
public:
	TextureIDInfo() = default;
	TextureIDInfo(TextureID _id, TextureManager* _manager) : m_textureID(_id), m_textureManager(_manager) {}	
	TextureIDInfo(const TextureIDInfo& _other) = default;
	TextureIDInfo& operator=(const TextureIDInfo& _other);

	TextureID GetTextureID() const;
	TextureManager* GetTextureManager() const;

	bool IsValid() const;
};


namespace TextureProperties {

	enum class TextureType : GLenum {
		
		TEXTURE_1D = GL_TEXTURE_1D,
		TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY, 
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_3D = GL_TEXTURE_3D,
		CUBEMAP = GL_TEXTURE_CUBE_MAP,
		CUBEMAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
		_UNDEFINED = 0
	};


	enum class ImageDataType : GLenum {
		UINT_8 = GL_UNSIGNED_BYTE,
		//INT_8 = GL_BYTE,
		//UINT_16 = GL_UNSIGNED_SHORT,
		//INT_16 = GL_SHORT,
		FLOAT_32 = GL_FLOAT
	};

	enum class ImageChannels : GLenum {
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		Depth = GL_DEPTH_COMPONENT
	};
	
	enum class TextureFormat : GLenum {
		R8 = GL_R8,

		RG8 = GL_RG8,
		
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8,
		
		SRGB8 = GL_SRGB8,
		SRGBA8 = GL_SRGB8_ALPHA8,

		R16F = GL_R16F,
		RG16F = GL_RG16F,
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
		NEAREST = GL_NEAREST,
		_UNDEFINED = GL_INVALID_ENUM

	};

	struct TextureProps {
		
		int m_mipmapCount							{ 1 };
		bool m_autogenerateMipmaps					{ false };
		TextureFormat m_internalImageFormat	{ TextureFormat::RGBA8 };
		ImageDataType m_pixelDataType				{ ImageDataType::UINT_8 };
		ImageChannels m_pixelFormat					{ ImageChannels::RGBA };

		WrapBehaviour m_wrapU		{ TextureProperties::WrapBehaviour::REPEAT };
		WrapBehaviour m_wrapV		{ TextureProperties::WrapBehaviour::REPEAT };

		FilterBehaviour m_filterMin { TextureProperties::FilterBehaviour::LINEAR };
		FilterBehaviour m_filterMag { TextureProperties::FilterBehaviour::LINEAR };
	};



	struct TextureSamplingProperties {
		WrapBehaviour m_wrapU;
		WrapBehaviour m_wrapV;

		FilterBehaviour m_filterMin;
		FilterBehaviour m_filterMag;
	};

	struct ImageUploadData {
		int m_mipLevel;
		glm::ivec3 m_dimensions{};
		void* m_textureData;
		bool m_textureIsFlipped { false };
	};

	using TextureUploadData = std::optional<std::vector<TextureProperties::ImageUploadData>>;

}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureType& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageDataType& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageChannels& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureFormat& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::WrapBehaviour& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::FilterBehaviour& _type);

namespace TextureCreation {


}



// Internal GPU class for texture
// handles raw allocation and upload logic
class TextureGPU {
public:
	TextureGPU(
		TextureProperties::TextureType _type, 
		glm::ivec3 _dims=glm::ivec3(1,1,1), 
		TextureProperties::TextureProps _props = TextureProperties::TextureProps{}
	);
	~TextureGPU();
	TextureGPU(TextureGPU&& _old) noexcept;
	TextureGPU& operator=(TextureGPU&& _old) noexcept;

	TextureGPU(TextureGPU& _old) = delete;
	TextureGPU& operator=(TextureGPU& _old) = delete;
public:
	const GLuint& GetTextureHandle() const;


	const TextureProperties::TextureType& GetTextureType() const;

	
	// - tex properties ------------------
	const TextureProperties::TextureFormat& GetInternalImageFormat() const;
	void SetInternalImageFormat(const TextureProperties::TextureFormat& _format);
	const TextureProperties::ImageDataType& GetDataType() const;
	void SetDataType(const TextureProperties::ImageDataType& _pixelDataType);
	const TextureProperties::ImageChannels& GetChannels() const;
	void SetChannels(const TextureProperties::ImageChannels& _pixelFormat);


	// - sampling behaviours -------------
	void SetWrapBehaviourU(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	const TextureProperties::WrapBehaviour& GetWrapBehaviourU() const;
	void SetWrapBehaviourV(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	const TextureProperties::WrapBehaviour& GetWrapBehaviourV() const;

	void SetFilterBehaviourMin(const TextureProperties::FilterBehaviour& _filterBehaviour);
	const TextureProperties::FilterBehaviour& GetFilterBehaviourMin() const;
	void SetFilterBehaviourMag(const TextureProperties::FilterBehaviour& _filterBehaviour);
	const TextureProperties::FilterBehaviour& GetFilterBehaviourMag() const;

	// - dimension setters ---------------
	void SetDimensions(glm::ivec3 _dims);
	const glm::ivec3& GetDimensions() const;
	const int& GetX() const;
	void SetX(const int& _val);

	const int& GetY() const;
	void SetY(const int& _val);
	
	const int& GetZ() const;
	void SetZ(const int& _val);


	// - pixel setter -------------------
	void SetPixelColor(glm::u8vec1 _col, glm::ivec3 _pixelPos);	// r
	void SetPixelColor(glm::u8vec2 _col, glm::ivec3 _pixelPos);	// rg
	void SetPixelColor(glm::u8vec3 _col, glm::ivec3 _pixelPos);	// rgb
	void SetPixelColor(glm::u8vec4 _col, glm::ivec3 _pixelPos);	// rgba


public:
	void Create();
	void Destroy();
	void Allocate();
	void Upload(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	bool NeedUpdate() const;
	bool NeedAllocate() const;
private:
	void UploadTexture2DData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void UploadCubemapData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void Upload3DTextureData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void UploadCubemapArrayData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void UploadTexture2DArrayData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;


protected:
	void UpdateTextureProperties() const;
protected:

	// 
	bool m_reallocateDirty{};
	bool m_samplingDirty{};
	bool m_allocated{};
	bool m_uploaded{};
	bool m_uploadNeedUpdate{};
	
	// 
	TextureProperties::TextureType m_textureType;
	GLuint m_glTextureHandle{};
	glm::ivec3 m_dimensions;
	TextureProperties::TextureProps m_textureProperties;
};



// handles
// texture manager can only construct directly. otherwise, you can only construct from a copy.
class Texture {
public:
	Texture(const TextureIDInfo& _info);
	Texture(const Texture& _other) = default;
	Texture& operator=(const Texture& _other) = default;

	bool IsValid() const;
	const TextureIDInfo& GetTextureIDInfo() const;

	// functions here assume you can access the texture.
	// getters particularly do not have safety checks so write accordingly
	void SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour);
	void SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour);
	TextureProperties::FilterBehaviour GetMinFilter() const;
	TextureProperties::FilterBehaviour GetMagFilter() const;


	void SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	void SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour);

	void SetInternalFormat(const TextureProperties::TextureFormat& _format); 
	void SetPixelDataType(const TextureProperties::ImageDataType& _pixelType);
	void SetPixelFormat(const TextureProperties::ImageChannels& _pixelFormat);

	void SetPixelColor(unsigned _col, int _x, int _y); // hexadecimal color
	void SetPixelColor(glm::vec4 _col, int _x, int _y);
protected:
	// be careful when using this as this assumes it is valid.
	TextureGPU& GetTextureGPU();
	const TextureGPU& GetTextureGPU() const;

protected:
	TextureProperties::TextureType m_textureType{}; // static and cannot be changed after creation; per type.
	TextureIDInfo m_textureIdInfo;
};
