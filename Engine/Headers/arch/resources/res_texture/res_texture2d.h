#pragma once
#include <arch/resources/res_texture/res_texture.h>



class Texture2D : public Texture {
public:
	Texture2D(int _width, int _height);

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	

	void SetDimensions(glm::ivec2 _dimensions);
	glm::ivec2 GetDimensions() const;
	void SetWidth(int _width);
	int GetWidth() const;
	void SetHeight(int _height);
	int GetHeight() const;


	void Upload(TextureProperties::TextureUploadData _imageData = std::nullopt);
};