#include <arch/resources/res_texture/res_texture2d.h>
#include <arch/resources/res_textureManager.h>


Texture2D::Texture2D(
	ResourceIdentifierArg _resHandle, 
	GPUResourceHandle _resGpuHandle
) : Texture(_resHandle, _resGpuHandle) 
{
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D;
}

void Texture2D::Load(std::filesystem::path _imgPath) {

}

glm::ivec2 Texture2D::GetDimensions() const {
	const GPU_Texture& tex = GetGPUTexture();
	const glm::vec3 dims = tex.GetDimensions();
	return { dims.x, dims.y };
}

void Texture2D::SetDimensions(glm::ivec2 _dims) {
	GPU_Texture& tex = GetGPUTexture();
	tex.SetDimensions({_dims.x, _dims.y, 1});
}


int Texture2D::GetWidth() const {
	const GPU_Texture& tex = GetGPUTexture();
	return tex.GetX();
}
int Texture2D::GetHeight() const {
	const GPU_Texture& tex = GetGPUTexture();
	return tex.GetY();
}

void Texture2D::SetWidth(int _width) {
	GPU_Texture& tex = GetGPUTexture();
	tex.SetX(_width);
}

void Texture2D::SetHeight(int _height) {
	GPU_Texture& tex = GetGPUTexture();
	tex.SetY(_height);
}

