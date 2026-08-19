#pragma once 
#include <array>
#include <arch/resources/res_texture/res_texture.h>


class CubemapRes : public TextureRes {
public:
	CubemapRes();

	int GetWidth() const;
	void SetWidth(int _width);
private:
	std::vector<ImageData> m_imageData;
};


struct CubemapHandle : public TextureHandle {
	inline CubemapHandle(ResourceIdentifierArg _arg) : TextureHandle(_arg) {}
	int GetWidth() const;
	void SetWidth(int _width);
};