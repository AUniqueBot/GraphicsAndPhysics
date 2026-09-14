#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/resources/res_resourceManager.h>
#include <graphics/device/gfx_gpuresourcemanager.h>

#include <serialization/serialize_metafile.h>
#include <serialization/serialize_jsonfile.h>


enum class GPUUploadBehaviour {
	IMMEDIATE,
	ON_REQUEST
};



class SpecializedResourceManager {
	using SerializerFunction = std::function<void(const Serialization::JSONValue&, ResourceHandle*)>;
public:
	SpecializedResourceManager(
		ResourceManager& _manager, 
		GPUResourceManager& _gpuManager
	) : m_resourceManager		{ _manager }, 
		m_gpuResourceManager	{ _gpuManager } {}
	
public:
	virtual void Init() {}
	virtual void Cleanup() {};

	// basic access
	bool Has(RES_ID _id) const;
	void Add(RES_ID _id);
	void Remove(RES_ID _id);
	std::unordered_set<RES_ID> List() const;

	// alias
	void SetResourceAlias(RES_ID _id, std::string _alias);
	void RemoveAliasForRes(RES_ID _id);
	RES_ID GetResIDFromAlias(std::string _alias) const;

	Serialization::MetafileData CreateMetafileData(const std::filesystem::path& _entry, std::shared_ptr<BaseResource> _res);

	virtual void LoadResource(const Serialization::MetafileData& _data);
	void RegisterFileExtension(std::string _ext);
	void DeregisterFileExtension(std::string _ext);
	bool AcceptsFileExtension(std::string _ext) const;


	void UpdateResourceMetafile(const std::filesystem::path& _file);

	const std::unordered_set<RES_ID>& GetResourcePool() const;

public:
	ResourceHandle CreateResource(const Serialization::JSONValue& _value);
	void RegisterTypenameToFunction(std::string _name, SerializerFunction _function);
protected:
	void AddResourceToPool(ResourceHandle _handle);

	ResourceIdentifier RegisterResource(std::shared_ptr<BaseResource> _res);
	std::string GetAliasForRes(RES_ID _id) const;

	void GenerateResourceMetafile(const std::filesystem::path& _file);
	

protected:
	// current set contained.
	std::unordered_set<RES_ID> m_resourceIdPool;

	std::unordered_map<std::string, RES_ID> m_aliasToResId;
	std::unordered_map<RES_ID, std::string> m_resIdToAlias;

	ResourceManager& m_resourceManager;
	GPUResourceManager& m_gpuResourceManager;
	std::unordered_set<std::string> m_registeredExtensions;


	// generate some stuff idk
	std::unordered_map<
		std::string, 
		SerializerFunction
	> m_factoryFunctions;

};

