#include <pch.h>
#include <stb_image.h>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/resources/res_gpu_resources/res_gpu_texture.h>
#include <arch/resources/res_textureManager.h>

#include <arch/core.h>

#include <util/util_color.h>

// MUST use opengl ver 4.5+ for DSA.


GPU_Texture& TextureRes::GetGPU_Texture() {
	TextureManager& texMgr = Core::GetInstance().GetAssetManager().GetTextureManager();
	return *texMgr.GetTexture(ResourceID());
}
const GPU_Texture& TextureRes::GetGPU_Texture() const {
	TextureManager& texMgr = Core::GetInstance().GetAssetManager().GetTextureManager();
	return *texMgr.GetTexture(ResourceID());
}


// -----------------------------------------------------------------


	
Texture::Texture(ResourceIdentifierArg _resIdArg) : TextureHandle(_resIdArg) {

}


void Texture::SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	ResourceManager& resManager = Core::GetInstance().GetResourceManager();
	
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());
	if (!texHandle) return;
	GPU_Texture& tex = *texHandle;
	tex.SetFilterBehaviourMin(_filterBehaviour);
}

void Texture::SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour) {
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());
	if (!texHandle) return;
	GPU_Texture& tex = *texHandle;
	tex.SetFilterBehaviourMag(_filterBehaviour);
}

TextureProperties::FilterBehaviour Texture::GetMinFilter() const {
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());
	return texHandle->GetFilterBehaviourMin(); 
}

TextureProperties::FilterBehaviour Texture::GetMagFilter() const { 
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());
	return texHandle->GetFilterBehaviourMag();
}

void Texture::SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	GPU_Texture& texHandle = GetGPU_Texture();
	texHandle.SetWrapBehaviourU(_wrapBehaviour);
}

void Texture::SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour) {
	GPU_Texture& texHandle = GetGPU_Texture();
	texHandle.SetWrapBehaviourV(_wrapBehaviour);
}


void Texture::SetInternalFormat(const TextureProperties::TextureFormat& _format) {
	GPU_Texture& texHandle = GetGPU_Texture();
	texHandle.SetInternalImageFormat(_format);
} 

void Texture::SetPixelColor(unsigned _col, int _x, int _y, int _z) {
	SetPixelColor(HexToVec4F(_col), _x, _y, _z);
}

void Texture::SetPixelColor(glm::vec4 _col, int _x, int _y, int _z) {
	if (!HandleIsValid()) return;
	GPU_Texture& tex{ GetGPU_Texture() };
	glm::u8vec4 colData{};
	colData.r = static_cast<unsigned char>(_col.r * 0xff);
	colData.g = static_cast<unsigned char>(_col.g * 0xff);
	colData.b = static_cast<unsigned char>(_col.b * 0xff);
	colData.a = static_cast<unsigned char>(_col.a * 0xff);
	tex.SetPixelColor(colData, glm::ivec3{_x, _y, _z});
}

 
GPU_Texture& Texture::GetGPU_Texture() {
	assert(HandleIsValid() && "No handle.");
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

const GPU_Texture& Texture::GetGPU_Texture() const {
	assert(HandleIsValid() && "No handle.");
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());
	assert(texHandle && "No such texture exists.");
	return *texHandle;
}

GLuint Texture::GetTextureHandle() const {
	TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	SparseSetView<GPU_Texture> texHandle = texManager.GetTexture(GetResourceID());

	if (!texHandle) return 0;
	const GPU_Texture& tex { *texHandle };
	return tex.GetTextureHandle();
}

bool Texture::TextureIsValid() const {
	if (!HandleIsValid()) return false;
	const TextureManager& texManager = Core::GetInstance().GetAssetManager().GetTextureManager();
	return texManager.TextureExists(GetResourceID());
}
