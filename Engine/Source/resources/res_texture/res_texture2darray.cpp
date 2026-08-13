#include <arch/resources/res_texture/res_texture2darray.h>



Texture2DArray::Texture2DArray(
	ResourceIdentifierArg _resHandle, 
	GPUResourceHandle _resGpuHandle
) : Texture(_resHandle, _resGpuHandle) 
{
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D_ARRAY;
}




glm::ivec2 Texture2DArray::GetDimensions() const {
	const GPU_Texture& tex = GetGPUTexture();
	glm::ivec3 dims = tex.GetDimensions();
	return { dims.x, dims.y };
}
void Texture2DArray::SetDimensions(glm::ivec2 _dims) {
	GPU_Texture& tex = GetGPUTexture();
	int layers = tex.GetDimensions().z;
	tex.SetDimensions({_dims.x, _dims.y, layers});
}


int Texture2DArray::GetLayers() const {
	return GetGPUTexture().GetDimensions().z;
}
void Texture2DArray::SetLayers(int _layers) {
	GPU_Texture& tex = GetGPUTexture();
	glm::ivec3 old = tex.GetDimensions();
	tex.SetZ(_layers);
}


int Texture2DArray::GetWidth() const {
	const GPU_Texture& tex = GetGPUTexture();
	return tex.GetX();
}
int Texture2DArray::GetHeight() const {
	const GPU_Texture& tex = GetGPUTexture();
	return tex.GetY();
}

void Texture2DArray::SetWidth(int _width) {
	GPU_Texture& tex = GetGPUTexture();
	tex.SetX(_width);
}

void Texture2DArray::SetHeight(int _height) {
	GPU_Texture& tex = GetGPUTexture();
	tex.SetY(_height);
}
