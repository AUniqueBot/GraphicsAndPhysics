#pragma once
#include <arch/resources/res_texture/res_texture.h>



class Texture2DRes : public TextureRes {
public:
	Texture2DRes();
	void Load(std::filesystem::path _imgPath);

	glm::ivec2 GetDimensions() const;
	void SetDimensions(glm::ivec2 _dims);

	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int _width);
	void SetHeight(int _height);


	void SetImageData(const ImageData& _imgData);
private:

};


using Texture2DHandle = ResourceHandleDerivedT<TextureRes, Texture2DRes>;
