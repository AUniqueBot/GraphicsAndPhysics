#include <arch/resources/res_texture/res_texture2d.h>
#include <arch/systems/sys_render_modules/sys_render_textureManager.h>


Texture2D::Texture2D(const TextureIDInfo& _info) : Texture(_info) {
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D;
}

void Texture2D::SetDimensions(glm::ivec2 _dims) {
	TextureGPU& tex = *m_textureIdInfo.GetTextureManager()->GetTexture(m_textureIdInfo.GetTextureID());
	tex.SetDimensions({_dims.x, _dims.y, 1});
}