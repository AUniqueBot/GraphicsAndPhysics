#include <pch.h>
#include <stb_image.h>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/resources/res_gpu_resources/res_gpu_texture.h>
#include <arch/resources/res_textureManager.h>

#include <arch/core.h>

#include <util/util_color.h>

// MUST use opengl ver 4.5+ for DSA.



// -----------------------------------------------------------------


	
Texture::Texture(ResourceIdentifierArg _resIdArg, GPUResourceHandle _handle) :
	TextureHandle(_resIdArg), 
	m_gpuResHandle{ _handle } 
{

}


void Texture::SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	
	// upload to update the data.
	GetGPUTexture().SetFilterBehaviourMin(_filterBehaviour);
}

void Texture::SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	GetGPUTexture().SetFilterBehaviourMag(_filterBehaviour);
}

TextureProperties::FilterBehaviour Texture::GetMinFilter() const {
	return GetGPUTexture().GetFilterBehaviourMin();
}

TextureProperties::FilterBehaviour Texture::GetMagFilter() const { 
	return GetGPUTexture().GetFilterBehaviourMag();
}

void Texture::SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	GPU_Texture& texHandle = GetGPUTexture();
	texHandle.SetWrapBehaviourU(_wrapBehaviour);
}

void Texture::SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	GPU_Texture& texHandle = GetGPUTexture();
	texHandle.SetWrapBehaviourV(_wrapBehaviour);
}


void Texture::SetInternalFormat(const TextureProperties::TextureFormat& _format) {
	GPU_Texture& texHandle = GetGPUTexture();
	texHandle.SetInternalImageFormat(_format);
} 

void Texture::SetPixelColor(unsigned _col, int _x, int _y, int _z) {
	SetPixelColor(HexToVec4F(_col), _x, _y, _z);
}

void Texture::SetPixelColor(glm::vec4 _col, int _x, int _y, int _z) {
	if (!HandleIsValid()) return;
	GPU_Texture& tex{ GetGPUTexture() };
	glm::u8vec4 colData{};
	colData.r = static_cast<unsigned char>(_col.r * 0xff);
	colData.g = static_cast<unsigned char>(_col.g * 0xff);
	colData.b = static_cast<unsigned char>(_col.b * 0xff);
	colData.a = static_cast<unsigned char>(_col.a * 0xff);
	tex.SetPixelColor(colData, glm::ivec3{_x, _y, _z});
}

 
GPU_Texture& Texture::GetGPUTexture() {
	assert(HandleIsValid() && "No handle.");
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

const GPU_Texture& Texture::GetGPUTexture() const {
	assert(HandleIsValid() && m_gpuResHandle.m_id != 0&& "No handle.");
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

GLuint Texture::GetTextureHandle() const {
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	if (!texHandle) return 0;
	const GPU_Texture& tex { *texHandle };
	return tex.GetTextureHandle();
}

bool Texture::TextureIsValid() const {
	if (!HandleIsValid() || m_gpuResHandle.m_id == C_GPURES_INVALID_ID) return false;
	GPUResourceManager& gpuMgr = Core::GetInstance().GetGPUResourceManager();
	SparseSetView<GPU_Texture> texHandle = gpuMgr.GetResource<GPU_Texture>(m_gpuResHandle);
	return static_cast<bool>(texHandle);
}

