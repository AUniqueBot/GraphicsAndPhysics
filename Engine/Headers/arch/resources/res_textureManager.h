#pragma once
#include <pch.h>
#include <queue>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/datatypes/type_sparseSet.h>

#include <arch/resources/res_texture/res_texture2d.h>
#include <arch/resources/res_texture/res_texture2darray.h>


class TextureManager {
public:



	friend class Texture;
	Texture2D LoadTexture(const std::filesystem::path& _path);
	Texture2D Create2DTexture(int width, int height, TextureProperties::TextureProps _props = {});

	// you can ignore these for the time being.
	void Create3DTexture(int width, int height, int depth, TextureProperties::TextureProps _props = {});
	Texture2DArray Create2DArrayTexture(int width, int height, int layers, TextureProperties::TextureProps _props = {});
	void CreateCubemapTexture(int width, int height, TextureProperties::TextureProps _props = {});

	
	SparseSetView<TextureGPU> GetTexture(TextureID _id);


	
public:
	void UpdateTextures();


private:
	bool TextureExists(TextureID _id) const;	
	void Resolve(TextureGPU& _texture);



	// u8 variant
	

private:

	// store textures.
	// for textures 
	SparseSet<TextureID, TextureGPU> m_storage; 

};

