#pragma once
#include <pch.h>
#include <queue>

#include <arch/resources/res_texture/res_texture.h>
#include <arch/datatypes/type_sparseSet.h>

#include <arch/resources/res_texture/res_texture2d.h>
#include <arch/resources/res_texture/res_texture2darray.h>

#include <arch/resources/res_specializedResourceManager.h>

class TextureManager : public SpecializedResourceManager {

public:
	TextureManager(ResourceManager& _mgr, GPUResourceManager& _gpuMgr) : SpecializedResourceManager(_mgr, _gpuMgr) {};

public:
	

	friend class Texture;
	Texture2D LoadTexture(const std::filesystem::path& _path);
	Texture2D Create2DTexture(int width, int height, TextureProperties::TextureProps _props = {});

	// you can ignore these for the time being.
	void Create3DTexture(int width, int height, int depth, TextureProperties::TextureProps _props = {});
	Texture2DArray Create2DArrayTexture(int width, int height, int layers, TextureProperties::TextureProps _props = {});
	void CreateCubemapTexture(int width, int height, TextureProperties::TextureProps _props = {});



	
public:


private:
	bool TextureExists(RES_ID _id) const;

};

