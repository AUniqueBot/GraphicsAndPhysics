#pragma once
#include <pch.h>
#include <queue>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/datatypes/type_sparseSet.h>

#include <arch/resources/res_texture/res_texture2d.h>


class TextureManager {
public:
	friend class Texture;
	Texture2D LoadTexture(const std::filesystem::path& _path);
	Texture2D Create2DTexture(int width, int height);

	// you can ignore these for the time being.
	TextureIDInfo Create3DTexture(int width, int height, int depth);
	TextureIDInfo Create2DArrayTexture(int width, int height, int layers);
	TextureIDInfo CreateCubemapTexture(int width, int height);

	
	SparseSetView<TextureGPU> GetTexture(TextureID _id);

	void DeleteTexture(TextureIDInfo _id);
private:
	
	TextureIDInfo GenerateTextureIDInfo();
	void ReclaimTextureIDInfo(TextureIDInfo _id);
	bool TextureExists(TextureID _id) const;	
	void Resolve(TextureGPU& _texture);
private:

	// store textures.
	// for textures 
	SparseSet<TextureID, TextureGPU> m_storage; 
	std::queue<TextureID> m_freeIds;
	TextureID m_nextID{ 1 };
};