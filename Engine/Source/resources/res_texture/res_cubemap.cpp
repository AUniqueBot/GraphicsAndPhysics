#include <arch/resources/res_texture/res_cubemap.h>
#include <arch/resources/res_gpu_resources/res_gpu_texture.h>

CubemapRes::CubemapRes() {
	m_imageData.clear();
	m_imageData.reserve(6);
}

int CubemapRes::GetWidth() const {
	m_dimensions.x;
	const GPU_Texture& tex = GetGPUTexture();
	return tex.GetX();
}

void CubemapRes::SetWidth(int _width) {
	if (_width == m_dimensions.x) return;

	GPU_Texture& tex = GetGPUTexture();
	m_dimensions.x = _width;
	tex.SetX(_width);
	 m_infoDirty = true;
}
