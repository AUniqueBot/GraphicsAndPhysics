#pragma once
#include <pch.h>
#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_textureManager.h>

class AssetManager {


public:
	// manager access.
	
	inline ShaderManager& GetShaderManager() { return m_shaderManager; }
	inline const ShaderManager& GetShaderManager() const { return m_shaderManager; }
	
	inline MaterialManager& GetMaterialManager() { return m_materialManager; }
	inline const MaterialManager& GetMaterialManager() const { return m_materialManager; }

	inline TextureManager& GetTextureManager() { return m_textureManager; }
	inline const TextureManager& GetTextureManager() const { return m_textureManager; }

private:
	ShaderManager m_shaderManager;
	MaterialManager m_materialManager;
	TextureManager m_textureManager;
};