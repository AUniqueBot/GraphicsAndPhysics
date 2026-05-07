#pragma once
#include <arch/resources/res_texture/res_texture.h>


class Texture2DArray : public Texture{
	Texture2DArray(int _width, int _height, int _layerCount);

	Texture2DArray(const Texture2DArray&) = delete;
	Texture2DArray& operator=(const Texture2DArray&) = delete;


	void SetDimensions(glm::ivec2 _dimensions);
	glm::ivec2 GetDimensions() const;
	void SetWidth(int _width);
	int GetWidth() const;
	void SetHeight(int _height);
	int GetHeight() const;

	void SetLayerCount(int _layerCount);
	int GetLayerCount() const;

};