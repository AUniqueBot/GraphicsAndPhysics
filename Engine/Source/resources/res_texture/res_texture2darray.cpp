#include <arch/resources/res_texture/res_texture2darray.h>
#include <arch/resources/res_gpu_resources/res_gpu_texture.h>


Texture2DArrayRes::Texture2DArrayRes() {
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D_ARRAY;
}

glm::ivec2 Texture2DArrayRes::GetDimensions() const {
	const GPU_Texture& tex = GetGPUTexture();
	glm::ivec3 dims = tex.GetDimensions();
	return { dims.x, dims.y };
}
void Texture2DArrayRes::SetDimensions(glm::ivec2 _dims) {
	if (m_dimensions.x == _dims.x && m_dimensions.y == _dims.y) return;
	m_dimensions.x = _dims.x;
	m_dimensions.y = _dims.y;
	if (HasValidGPUResourceHandle()) {
		GPU_Texture& tex = GetGPUTexture();
		int layers = tex.GetDimensions().z;
		tex.SetDimensions({_dims.x, _dims.y, layers});
	}
	else {
		m_infoDirty = true;
	}
}


int Texture2DArrayRes::GetLayers() const {
	return GetGPUTexture().GetDimensions().z;
}
void Texture2DArrayRes::SetLayers(int _layers) {
	if (m_dimensions.z == _layers) return;
	m_dimensions.z = _layers;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetZ(_layers);
	}
	else {
		m_infoDirty = true;
	}
}


int Texture2DArrayRes::GetWidth() const {
	return HasValidGPUResourceHandle() ?
		GetGPUTexture().GetX() : m_dimensions.x;
}
int Texture2DArrayRes::GetHeight() const {
	return HasValidGPUResourceHandle() ?
		GetGPUTexture().GetY() : m_dimensions.y;
}

void Texture2DArrayRes::SetWidth(int _width) {
	if (m_dimensions.x == _width) return;
	m_dimensions.x = _width;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetX(_width);
	}
	else {
		m_infoDirty = true;
	}
}

void Texture2DArrayRes::SetHeight(int _height) {
	if (m_dimensions.y == _height) return;
	m_dimensions.y = _height;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetY(_height);
	}
	else {
		m_infoDirty = true;
	}
}
