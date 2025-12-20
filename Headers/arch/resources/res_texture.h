#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>

class Texture : public Resource {
private:
public:
	enum WRAP_BEHAVIOUR : int {
		REPEAT			= GL_REPEAT,
		MIRROR_REPEAT	= GL_MIRRORED_REPEAT,
		TO_EDGE			= GL_CLAMP_TO_EDGE,
		TO_BORDER		= GL_CLAMP_TO_BORDER
	};

	enum FILTER_BEHAVIOUR : int {
		LINEAR			= GL_LINEAR,
		NEAREST			= GL_NEAREST
	};


	// image channel count. automatic.
	enum class IMAGE_FORMAT : int {
		RGB				= GL_RGB,
		RGBA			= GL_RGBA
	};

	// how to interpret the image.
	enum class IMAGE_FORMAT_I : int {
		RGB				= GL_RGB,
		RGBA			= GL_RGBA
	};


	using TEX_ID = unsigned;
public:

	Texture(std::filesystem::path _newImage = "") : Resource(TEXTURE), m_resPath{_newImage} {}



	// TODO - Auto register files 

	// function loads data into GPU.
	bool Load();


	// - settings -------------------
	void SetWrappingBehavior(WRAP_BEHAVIOUR _h, WRAP_BEHAVIOUR _v);
	void SetWrappingBehaviorH(WRAP_BEHAVIOUR _setting);
	void SetWrappingBehaviorV(WRAP_BEHAVIOUR _setting);

	void SetFilterBehavior(FILTER_BEHAVIOUR _h, FILTER_BEHAVIOUR _v);
	void SetFilterBehaviorH(FILTER_BEHAVIOUR _setting);
	void SetFilterBehaviorV(FILTER_BEHAVIOUR _setting);
	void SetMipmapCount(int _setting);
	void FlipVertically(bool _setting);

	// - getters --------------------
	TEX_ID GetTextureID() const		{ return m_textureID; }
	glm::vec2 GetDimensions() const { return m_dimensions; };



private:
	bool BindTexture() {
		if (m_textureID != 0) glBindTexture(GL_TEXTURE_2D, m_textureID);
		return m_textureID != 0;
	}
	bool LoadImage();

	void UpdateResourceParameters();

private:
	
	// - engine stuff --------------------------------------------
	std::filesystem::path m_resPath;
	TEX_ID m_textureID		{ 0 };
	
	// - image properties ------------------------
	glm::vec2 m_dimensions				{ 0, 0 }; // automatically read on loading of image.
	
	int m_channelCount					{ 0 };

	WRAP_BEHAVIOUR m_wrapU				{ REPEAT };
	WRAP_BEHAVIOUR m_wrapV				{ REPEAT };
	
	FILTER_BEHAVIOUR m_filterU			{ LINEAR };
	FILTER_BEHAVIOUR m_filterV			{ LINEAR };
	
	bool m_flipImage					{ false };
	int m_mipmapCount					{ 4 }; // default 4, nice number.

	IMAGE_FORMAT m_imageFormat			{ IMAGE_FORMAT::RGB };
	IMAGE_FORMAT_I m_imageFormatI		{ IMAGE_FORMAT_I::RGB };


	// - dirty flags ------------------------------
	bool m_wrapUDirty					{ true };
	bool m_wrapVDirty					{ true };
	bool m_filterUDirty					{ true };
	bool m_filterVDirty					{ true };
	


	// unused.
	bool m_resDirty						{ true };
};


/*

	

*/