#pragma once
#include <arch/resources/res_texture/res_texture.h>


class Texture2DArrayRes  : public TextureRes {
public: 
	Texture2DArrayRes();
	glm::ivec2 GetDimensions() const;
	void SetDimensions(glm::ivec2 _dims);
	int GetLayers() const;
	void SetLayers(int _layers);

	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int _width);
	void SetHeight(int _height);
private:
	std::vector<ImageData> m_imageData;
};

using Texture2DArrayHandle = ResourceHandleDerivedT<TextureRes, Texture2DArrayRes>;