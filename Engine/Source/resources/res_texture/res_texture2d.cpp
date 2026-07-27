#include <arch/resources/res_texture/res_texture2d.h>
#include <arch/systems/sys_render_modules/sys_render_textureManager.h>


Texture2D::Texture2D(const TextureIDInfo& _info) : Texture(_info) {
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D;
}

glm::ivec2 Texture2D::GetDimensions() const {
	const TextureGPU& tex = GetTextureGPU();
	const glm::vec3 dims = tex.GetDimensions();
	return { dims.x, dims.y };
}

void Texture2D::SetDimensions(glm::ivec2 _dims) {
	TextureGPU& tex = GetTextureGPU();
	tex.SetDimensions({_dims.x, _dims.y, 1});
}


int Texture2D::GetWidth() const {
	const TextureGPU& tex = GetTextureGPU();
	return tex.GetX();
}
int Texture2D::GetHeight() const {
	const TextureGPU& tex = GetTextureGPU();
	return tex.GetY();
}

void Texture2D::SetWidth(int _width) {
	TextureGPU& tex = GetTextureGPU();
	tex.SetX(_width);
}

void Texture2D::SetHeight(int _height) {
	TextureGPU& tex = GetTextureGPU();
	tex.SetY(_height);
}

