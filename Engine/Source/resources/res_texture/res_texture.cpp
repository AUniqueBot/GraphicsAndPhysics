#include <pch.h>
#include <stb_image.h>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/resources/res_gpu_resources/res_gpu_texture.h>
#include <arch/resources/res_textureManager.h>

#include <arch/core.h>

#include <util/util_color.h>

// MUST use opengl ver 4.5+ for DSA.



// -----------------------------------------------------------------

const glm::ivec3& TextureRes::GetDimensions() const {
	return m_dimensions;
}

const TextureProperties::TextureType& TextureRes::GetTextureType() const {
	return m_textureType;
}



void TextureRes::SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	if (_filterBehaviour == m_textureProps.m_filterMin) return;
	m_textureProps.m_filterMin = _filterBehaviour;
	if (HasValidGPUResourceHandle())
		GetGPUTexture().SetFilterBehaviourMin(_filterBehaviour);
}

void TextureRes::SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	if (_filterBehaviour == m_textureProps.m_filterMag) return;
	m_textureProps.m_filterMag = _filterBehaviour;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetFilterBehaviourMag(_filterBehaviour);
	}
	else {
		m_infoDirty = true;
	}
}

TextureProperties::FilterBehaviour TextureRes::GetMinFilter() const {
	return HasValidGPUResourceHandle() ? 
		GetGPUTexture().GetFilterBehaviourMin() : m_textureProps.m_filterMin;
}

TextureProperties::FilterBehaviour TextureRes::GetMagFilter() const { 
	return HasValidGPUResourceHandle() ? 
		GetGPUTexture().GetFilterBehaviourMag() : m_textureProps.m_filterMag;
}

void TextureRes::SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	if (m_textureProps.m_wrapU == _wrapBehaviour) return;
	m_textureProps.m_wrapU = _wrapBehaviour;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetWrapBehaviourU(_wrapBehaviour);
	}
	else {
		m_infoDirty = true;
	}
}

void TextureRes::SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	if (m_textureProps.m_wrapV == _wrapBehaviour) return;
	m_textureProps.m_wrapV = _wrapBehaviour;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetWrapBehaviourV(_wrapBehaviour);
	}
	else {
		m_infoDirty = true;
	}
}


void TextureRes::SetInternalFormat(const TextureProperties::TextureFormat& _format) {
	if (m_textureProps.m_internalImageFormat == _format) return;
	m_textureProps.m_internalImageFormat = _format;
	if (HasValidGPUResourceHandle()) {
		GetGPUTexture().SetInternalImageFormat(_format);
	}
	else {
		m_infoDirty = true;
	}
} 


// - direct gpures access functions -------------
void TextureRes::SetPixelColor(unsigned _col, int _x, int _y, int _z) {
	SetPixelColor(Color:: HexToVec4F(_col), _x, _y, _z);
}

void TextureRes::SetPixelColor(glm::vec4 _col, int _x, int _y, int _z) {
	if (!HasValidGPUResourceHandle()) return;
	GPU_Texture& tex{ GetGPUTexture() };
	glm::u8vec4 colData{};
	colData.r = static_cast<unsigned char>(_col.r * 0xff);
	colData.g = static_cast<unsigned char>(_col.g * 0xff);
	colData.b = static_cast<unsigned char>(_col.b * 0xff);
	colData.a = static_cast<unsigned char>(_col.a * 0xff);
	tex.SetPixelColor(colData, glm::ivec3{_x, _y, _z});
}

 
GPU_Texture& TextureRes::GetGPUTexture() {
	assert(HasValidGPUResourceHandle() && "No handle.");
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

const GPU_Texture& TextureRes::GetGPUTexture() const {
	assert(HasValidGPUResourceHandle() && m_gpuResHandle.m_id != 0&& "No handle.");
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

GLuint TextureRes::GetTextureHandle() const {
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	if (!texHandle) return 0;
	const GPU_Texture& tex { *texHandle };
	return tex.GetTextureHandle();
}



bool TextureRes::InfoDirty() const {
	return m_infoDirty;
}


void TextureRes::SetGPUResourceHandle(GPUResourceHandle _handle) {
	m_gpuResHandle = _handle;
}

bool TextureRes::HasValidGPUResourceHandle() const {
	return m_gpuResHandle.IsValid();
}


const std::vector<ImageData>& TextureRes::GetImageData() const {
	return m_imageData;
}

const TextureProperties::TextureProps& TextureRes::GetTextureProps() const {
	return m_textureProps;
}

void TextureRes::SetTextureProps(const TextureProperties::TextureProps& _props) {
	m_textureProps = _props;
	m_infoDirty = true;
}

// --------------------------------------------------------------------------------------------







