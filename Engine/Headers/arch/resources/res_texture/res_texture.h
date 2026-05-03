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

	struct TextureProps {

		int m_mipmapCount{ 1 };
		bool m_autogenerateMipmaps{};
		InternalImageFormat m_internalImageFormat;
		PixelDataType m_pixelDataType;
		PixelFormat m_pixelFormat;

		WrapBehaviour m_wrapU;
		WrapBehaviour m_wrapV;

		FilterBehaviour m_filterMin;
		FilterBehaviour m_filterMag;
	};


	struct TextureSamplingProperties {
		WrapBehaviour m_wrapU;
		WrapBehaviour m_wrapV;

		FilterBehaviour m_filterMin;
		FilterBehaviour m_filterMag;
	};

	struct ImageUploadData {
		glm::ivec3 m_dimensions{};
		void* m_textureData;
	};

	using TextureUploadData = std::optional<std::vector<TextureProperties::ImageUploadData>>;

}


namespace TextureCreation {


}

class Texture : public Resource<Texture> {
public:
	Texture(TextureProperties::TextureType _type);
	Texture(TextureProperties::TextureProps _props);
	~Texture();
	Texture(Texture&& _old) noexcept;
	Texture& operator=(Texture&& _old) noexcept;

	Texture(Texture& _old) = delete;
	Texture& operator=(Texture& _old) = delete;
public:
	const GLuint& GetTextureID() const;


	const TextureProperties::TextureType& GetTextureType() const;

	
	// - tex properties ------------------
	const TextureProperties::InternalImageFormat& GetInternalImageFormat() const;
	void SetInternalImageFormat(const TextureProperties::InternalImageFormat& _format);
	const TextureProperties::PixelDataType& GetPixelDataType() const;
	void SetPixelDataType(const TextureProperties::PixelDataType& _pixelDataType);
	const TextureProperties::PixelFormat& GetPixelFormat() const;
	void SetPixelFormat(const TextureProperties::PixelFormat& _pixelFormat);


	// - sampling behaviours -------------
	void SetWrapBehaviourU(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	const TextureProperties::WrapBehaviour& GetWrapBehaviourU() const;
	void SetWrapBehaviourV(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	const TextureProperties::WrapBehaviour& GetWrapBehaviourV() const;

	void SetFilterBehaviourMin(const TextureProperties::FilterBehaviour& _filterBehaviour);
	const TextureProperties::FilterBehaviour& GetFilterBehaviourMin() const;
	void SetFilterBehaviourMag(const TextureProperties::FilterBehaviour& _filterBehaviour);
	const TextureProperties::FilterBehaviour& GetFilterBehaviourMag() const;

public:
	void Create();
	void Destroy();
	void Allocate();
protected:
	void UpdateTextureProperties();
protected:
	bool m_reallocateDirty{};
	bool m_samplingDirty{};
	bool m_allocated{};
	bool m_uploaded{};
	TextureProperties::TextureType m_textureType{};
	GLuint m_textureId{};
	glm::ivec3 m_dimensions;
	TextureProperties::TextureProps m_textureProperties;
};


