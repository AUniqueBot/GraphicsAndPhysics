#pragma once
#include <arch/resources/res_texture/res_texture.h>



class Texture2D : public Texture {
public:
	Texture2D(ResourceIdentifierArg _resHandle = std::nullopt, GPUResourceHandle _resGpuHandle = {});
	Texture2D(const Texture2D&) = default;
	Texture2D& operator=(const Texture2D&) = default;


	glm::ivec2 GetDimensions() const;
	void SetDimensions(glm::ivec2 _dims);

	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int _width);
	void SetHeight(int _height);


private:

};