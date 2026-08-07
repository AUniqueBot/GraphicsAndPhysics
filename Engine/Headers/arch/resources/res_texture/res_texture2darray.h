#pragma once
#include <arch/resources/res_texture/res_texture.h>


class Texture2DArray  : public Texture {
public: 
	Texture2DArray(ResourceIdentifierArg _resHandle = std::nullopt);
	Texture2DArray(const Texture2DArray&) = default;
	Texture2DArray& operator=(const Texture2DArray&) = default;



	glm::ivec2 GetDimensions() const;
	void SetDimensions(glm::ivec2 _dims);
	int GetLayers() const;
	void SetLayers(int _layers);


	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int _width);
	void SetHeight(int _height);

};