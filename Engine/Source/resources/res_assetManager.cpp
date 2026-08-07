#include <arch/resources/res_assetManager.h>


void AssetManager::Init() {
	m_shaderManager.Init();
	m_shaderPrgManager.Init();
	m_materialManager.Init();
	m_textureManager.Init();
}

ShaderManager& AssetManager::GetShaderManager() { 
	return m_shaderManager; 
}
const ShaderManager& AssetManager::GetShaderManager() const { 
	return m_shaderManager; 
}

ShaderProgramManager& AssetManager::GetShaderProgramManager() {
	return m_shaderPrgManager;
}

const ShaderProgramManager& AssetManager::GetShaderProgramManager() const {
	return m_shaderPrgManager;
}

MaterialManager& AssetManager::GetMaterialManager() { 
	return m_materialManager; 
}
const MaterialManager& AssetManager::GetMaterialManager() const { 
	return m_materialManager; 
}

TextureManager& AssetManager::GetTextureManager() { 
	return m_textureManager; 
}
const TextureManager& AssetManager::GetTextureManager() const { 
	return m_textureManager; 
}