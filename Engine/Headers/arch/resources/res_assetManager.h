#pragma once
#include <pch.h>
#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_textureManager.h>
#include <arch/resources/res_meshManager.h>

#include <serialization/serialize_metafilereader.h>



class AssetManager {


public:
	// manager access.
	AssetManager(ResourceManager& _mgr, GPUResourceManager& _gpuMgr) : 
		m_materialManager	{ _mgr, _gpuMgr }, 
		m_shaderManager		{ _mgr, _gpuMgr }, 
		m_textureManager	{ _mgr, _gpuMgr } ,
		m_shaderPrgManager	{ _mgr, _gpuMgr, m_shaderManager },
		m_meshManager		{ _mgr, _gpuMgr }
	{}

public:
	void Init();
	void Cleanup();
	void ScanResourcesInPath(const std::filesystem::path& _path, bool _recursive = true);

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

public:
	void SaveMetafileData(const Serialization::MetafileData& _data);
	void LoadResource(const Serialization::MetafileData& _data);

private:
	ShaderManager m_shaderManager;
	ShaderProgramManager m_shaderPrgManager;
	MaterialManager m_materialManager;
	TextureManager m_textureManager;
	MeshManager m_meshManager;
private:
	Serialization::MetafileSerializer m_serializer;

};