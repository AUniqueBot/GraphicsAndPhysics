#include <arch/resources/res_texture/res_texture2d.h>
#include <arch/resources/res_textureManager.h>


Texture2DRes::Texture2DRes() {
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D;
	m_imageData.resize(1);
}

void Texture2DRes::Load(std::filesystem::path _imgPath) {

}

glm::ivec2 Texture2DRes::GetDimensions() const {
	if (!m_gpuResHandle.IsValid()) 
		return { m_dimensions.x, m_dimensions.y };	
	const GPU_Texture& tex = GetGPUTexture();
	const glm::vec3 dims = tex.GetDimensions();
	return { dims.x, dims.y };
}

void Texture2DRes::SetDimensions(glm::ivec2 _dims) {
	if (m_dimensions.x == _dims.x && m_dimensions.y == _dims.y) {
		return;
	}
	m_dimensions.x = _dims.x;
	m_dimensions.y = _dims.y;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetDimensions({_dims.x, _dims.y, 1});
	}
	else {
		m_infoDirty = true;
	}
}


int Texture2DRes::GetWidth() const {
	return HasValidGPUResourceHandle() ? GetGPUTexture().GetX() : m_dimensions.x;
}
int Texture2DRes::GetHeight() const {
	return HasValidGPUResourceHandle() ? GetGPUTexture().GetY() : m_dimensions.y;
}

void Texture2DRes::SetWidth(int _width) {
	if (m_dimensions.x == _width) return;
	m_dimensions.x = _width;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetX(_width);
	}
	else {
		m_infoDirty = true;
	}
}

void Texture2DRes::SetHeight(int _height) {
	if (m_dimensions.y == _height) return;
	m_dimensions.y = _height;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetX(_height);
	}
	else {
		m_infoDirty = true;
	}
}

void Texture2DRes::SetImageData(const ImageData& _imgData) {
	m_imageData[0] = _imgData;
	m_infoDirty = true;
}



