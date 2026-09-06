#pragma once
#include <pch.h>
#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_textureManager.h>
#include <arch/resources/res_meshManager.h>

#include <serialization/serialize_helperfunctions.h>
#include <serialization/serialize_metafilereader.h>



class AssetManager {


public:
	// manager access.
	AssetManager(ResourceManager& _mgr, GPUResourceManager& _gpuMgr) :
		m_resourceManager	{ _mgr },
		m_gpuResourceManager{ _gpuMgr }

		//m_materialManager	{ _mgr, _gpuMgr }, 
		//m_shaderManager		{ _mgr, _gpuMgr }, 
		//m_textureManager	{ _mgr, _gpuMgr } ,
		//m_shaderPrgManager	{ _mgr, _gpuMgr, m_shaderManager },
		//m_meshManager		{ _mgr, _gpuMgr }
	{}

public:
	void Init();
	void Cleanup();
	void ScanResourcesInPath(const std::filesystem::path& _path, bool _recursive = true);

public:
	template <std::derived_from<BaseResource> ResType, std::derived_from<SpecializedResourceManager> ManagerType>
	void RegisterManager() {
		RESTYPE_ID typeId = ResType::GetResourceTypeID();
		if (!m_managerList.At(typeId)) {
			auto manager = std::make_shared<ManagerType>(m_resourceManager, m_gpuResourceManager);
			manager->Init();
			m_managerList.Add(std::move(manager), ResType::GetResourceTypeID());
		}
		else {
			LOG_WARN("ignoring attempt to overwrite existing manager for resource. " 
				<< nameof::nameof_short_type<ResType>());
		}
	}



public:
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
	//ShaderManager m_shaderManager;
	//ShaderProgramManager m_shaderPrgManager;
	//MaterialManager m_materialManager;
	//TextureManager m_textureManager;
	//MeshManager m_meshManager;

private:
	// references to core managers.
	ResourceManager& m_resourceManager; 
	GPUResourceManager& m_gpuResourceManager;
	SparseSet<RESTYPE_ID, std::shared_ptr<SpecializedResourceManager>> m_managerList;
};