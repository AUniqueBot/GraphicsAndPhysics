#pragma once
#include <pch.h>
#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_textureManager.h>

class AssetManager {


public:
	// manager access.
	AssetManager(ResourceManager& _mgr) : 
		m_materialManager	{ _mgr }, 
		m_shaderManager		{ _mgr }, 
		m_textureManager	{ _mgr } 
	
	{}

public:
	void Init();
	void Cleanup();

	ShaderManager& GetShaderManager();
	const ShaderManager& GetShaderManager() const;
	
	MaterialManager& GetMaterialManager();
	const MaterialManager& GetMaterialManager() const;

	TextureManager& GetTextureManager();
	const TextureManager& GetTextureManager() const;

private:
	ShaderManager m_shaderManager;
	MaterialManager m_materialManager;
	TextureManager m_textureManager;
};