#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


class Texture;
namespace TextureCreation {

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


	// image channel count. automatic.
	enum class ImageFormat : int {
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};

	// how image is internally stored.
	enum class ImageFormatInternal : int {
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};


	struct TextureInfo {
		glm::ivec2 m_resolution = { 1, 1 };

		GLenum m_internalFormat = GL_RGBA8;   // GPU storage format
		GLenum m_format = GL_RGBA;    // input data format
		GLenum m_type = GL_UNSIGNED_BYTE;

		int m_mipLevels = 1;

		// sampling
		GLenum m_wrapU = GL_REPEAT;
		GLenum m_wrapV = GL_REPEAT;

		GLenum m_minFilter = GL_LINEAR;
		GLenum m_magFilter = GL_LINEAR;

		bool m_generateMipmaps = true;
	};



	Texture CreateTexture(
		const TextureInfo& _info,
		const void* _data = nullptr
	);
	Texture CreateTextureFromFile(
		const std::filesystem::path& path
	);
}



class Texture : public Resource<Texture> {
private:
public:



	using TEX_ID = unsigned;
public:
	// either path or image.


	// - settings -------------------
	void SetWrappingBehavior(TextureCreation::WrapBehaviour _u, TextureCreation::WrapBehaviour _v);
	void SetWrappingBehaviorU(TextureCreation::WrapBehaviour _setting);
	void SetWrappingBehaviorV(TextureCreation::WrapBehaviour _setting);
	std::pair<TextureCreation::WrapBehaviour, TextureCreation::WrapBehaviour> GetWrappingBehaviour() const;
	const TextureCreation::WrapBehaviour& GetWrappingBehaviourU() const;
	const TextureCreation::WrapBehaviour& GetWrappingBehaviourV() const;



	void SetFilterBehavior(TextureCreation::FilterBehaviour _h, TextureCreation::FilterBehaviour _v);
	void SetFilterBehaviorU(TextureCreation::FilterBehaviour _setting);
	void SetFilterBehaviorV(TextureCreation::FilterBehaviour _setting);
	std::pair<TextureCreation::FilterBehaviour, TextureCreation::FilterBehaviour> GetFilterBehaviour() const;
	const TextureCreation::FilterBehaviour& GetFilterBehaviourU() const;
	const TextureCreation::FilterBehaviour& GetFilterBehaviourV() const;

	
	void SetMipmapCount(int _setting);
	void FlipVertically(bool _setting);



	// - getters --------------------
	void SetTextureId(const TEX_ID& _textureId); 
	TEX_ID GetTextureID() const;
	glm::vec2 GetDimensions() const;


	
private:
	bool BindTexture() const;

	void UpdateResourceParameters();

private:
	
	// - engine stuff --------------------------------------------
	TEX_ID m_textureID		{ 0 };
	
	// - image properties ------------------------
	glm::vec2 m_dimensions				{ 0, 0 }; // automatically read on loading of image.
	
	int m_channelCount					{ 0 };


	TextureCreation::WrapBehaviour m_wrapU				{ TextureCreation::WrapBehaviour::REPEAT };
	TextureCreation::WrapBehaviour m_wrapV				{ TextureCreation::WrapBehaviour::REPEAT };
	
	TextureCreation::FilterBehaviour m_filterU			{ TextureCreation::FilterBehaviour::LINEAR };
	TextureCreation::FilterBehaviour m_filterV			{ TextureCreation::FilterBehaviour::LINEAR };
	
	bool m_flipImage					{ false };
	int m_mipmapCount					{ 4 }; // default 4, nice number.

	TextureCreation::ImageFormat m_imageFormat				{ TextureCreation::ImageFormat::RGB };
	TextureCreation::ImageFormatInternal m_imageFormatI	{ TextureCreation::ImageFormatInternal::RGB };


};


