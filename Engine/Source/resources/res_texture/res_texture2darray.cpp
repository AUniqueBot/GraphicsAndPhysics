#include <arch/resources/res_texture/res_texture2darray.h>



Texture2DArray::Texture2DArray(std::shared_ptr<TextureRes> _resHandle) : Texture(_resHandle) {
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D_ARRAY;
}



glm::ivec2 Texture2DArray::GetDimensions() const {
	const TextureGPU& tex = GetTextureGPU();
	glm::ivec3 dims = tex.GetDimensions();
	return { dims.x, dims.y };
}
void Texture2DArray::SetDimensions(glm::ivec2 _dims) {
	TextureGPU& tex = GetTextureGPU();
	int layers = tex.GetDimensions().z;
	tex.SetDimensions({_dims.x, _dims.y, layers});
}


int Texture2DArray::GetLayers() const {
	return GetTextureGPU().GetDimensions().z;
}
void Texture2DArray::SetLayers(int _layers) {
	TextureGPU& tex = GetTextureGPU();
	glm::ivec3 old = tex.GetDimensions();
	tex.SetZ(_layers);
}


int Texture2DArray::GetWidth() const {
	const TextureGPU& tex = GetTextureGPU();
	return tex.GetX();
}
int Texture2DArray::GetHeight() const {
	const TextureGPU& tex = GetTextureGPU();
	return tex.GetY();
}

void Texture2DArray::SetWidth(int _width) {
	TextureGPU& tex = GetTextureGPU();
	tex.SetX(_width);
}

void Texture2DArray::SetHeight(int _height) {
	TextureGPU& tex = GetTextureGPU();
	tex.SetY(_height);
}
