#pragma once
#include <pch.h>
#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_textureManager.h>
#include <arch/resources/res_meshManager.h>

class AssetManager {


public:
	// manager access.
	AssetManager(ResourceManager& _mgr) : 
		m_materialManager	{ _mgr }, 
		m_shaderManager		{ _mgr }, 
		m_textureManager	{ _mgr } ,
		m_shaderPrgManager	{ _mgr, m_shaderManager },
		m_meshManager		{ _mgr }
	{}

public:
	void Init();
	void Cleanup();

	ShaderManager& GetShaderManager();
	const ShaderManager& GetShaderManager() const;
	
	ShaderProgramManager& GetShaderProgramManager();
	const ShaderProgramManager& GetShaderProgramManager() const;


	MaterialManager& GetMaterialManager();
	const MaterialManager& GetMaterialManager() const;

	TextureManager& GetTextureManager();
	const TextureManager& GetTextureManager() const;

	MeshManager& GetMeshManager();
	const MeshManager& GetMeshManager() const;

private:
	ShaderManager m_shaderManager;
	ShaderProgramManager m_shaderPrgManager;
	MaterialManager m_materialManager;
	TextureManager m_textureManager;
	MeshManager m_meshManager;

};