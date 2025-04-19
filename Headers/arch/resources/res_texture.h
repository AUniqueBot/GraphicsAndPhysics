#pragma once
#include <pch.h>


class Texture {
private:
public:
	enum WRAP_BEHAVIOUR {
		REPEAT,
		MIRROR_REPEAT,
		TO_EDGE,
		TO_BORDER
	};

	enum FILTER_BEHAVIOUR {
		LINEAR,
		NEAREST
	};



	enum class IMAGE_FORMAT {
		RGB,
		RGBA
	};

	enum class IMAGE_FORMAT_I {
		RGB,
		RGBA
	};



	using TEX_ID = unsigned;

public:

	Texture(std::filesystem::path _newImage) : m_imagePath{ _newImage }  { }



	// TODO - Auto register files 
	bool Load();


	// - settings -------------------
	void SetWrappingBehavior(WRAP_BEHAVIOUR _h, WRAP_BEHAVIOUR _v);
	void SetWrappingBehaviorH(WRAP_BEHAVIOUR _setting);
	void SetWrappingBehaviorV(WRAP_BEHAVIOUR _setting);

	void SetFilterBehavior(FILTER_BEHAVIOUR _h, FILTER_BEHAVIOUR _v);
	void SetFilterBehaviorH(FILTER_BEHAVIOUR _setting);
	void SetFilterBehaviorV(FILTER_BEHAVIOUR _setting);

	void FlipVertically(bool _setting);

	// - getters --------------------
	TEX_ID GetTextureID() const		{ return m_textureID; }
	glm::vec2 GetDimensions() const { return m_dimensions; };



private:
	TEX_ID m_textureID		{ 0 };
	glm::vec2 m_dimensions	{ 0, 0 };

	int m_channelCount		{};

	std::filesystem::path m_imagePath;
};
