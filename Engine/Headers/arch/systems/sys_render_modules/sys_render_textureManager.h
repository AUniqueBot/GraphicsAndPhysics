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
	Texture2D Create2DTexture(int width, int height, TextureProperties::TextureProps _props = {});

	// you can ignore these for the time being.
	TextureIDInfo Create3DTexture(int width, int height, int depth, TextureProperties::TextureProps _props = {});
	TextureIDInfo Create2DArrayTexture(int width, int height, int layers, TextureProperties::TextureProps _props = {});
	TextureIDInfo CreateCubemapTexture(int width, int height, TextureProperties::TextureProps _props = {});

	
	SparseSetView<TextureGPU> GetTexture(TextureID _id);

	void DeleteTexture(TextureIDInfo _id);
private:
	
	TextureIDInfo GenerateTextureIDInfo();
	void ReclaimTextureIDInfo(TextureIDInfo _id);
	bool TextureExists(TextureID _id) const;	
	void Resolve(TextureGPU& _texture);



	// u8 variant
	

private:

	// store textures.
	// for textures 
	SparseSet<TextureID, TextureGPU> m_storage; 
	std::queue<TextureID> m_freeIds;
	TextureID m_nextID{ 1 };
};

