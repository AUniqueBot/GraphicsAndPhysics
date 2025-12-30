#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


/*!
	@brief a class designed to manage resources used in the application 
*/
class ResourceManager {

private:

public:
	ResourceManager();
public:
	
	void Init();
	
	void ScanResourcesInPath(std::filesystem::path _filePath, bool _recursive = true);
	
	// optional path.
	RES_ID AddResource(std::shared_ptr<Resource> _resource, std::filesystem::path _path);
	void RemoveResource(RES_ID _id); 
	std::shared_ptr<Resource> GetResource(RES_ID _id);


	//void RemoveResource(std::string _name); 



	

	// registration
	void RegisterFileExtension(std::string _extension, Resource::RESOURCE_TYPE _type);
	void DeregisterFileExtension(std::string _extension);
	Resource::RESOURCE_TYPE GetResourceType(std::string _extension) const;
	
	// file packing
	void PackResources();

public:
	void LoadPaths();

	void LoadDefaultResources();


	void LoadResource(std::filesystem::path _filePath);
private:
	static RES_ID GenerateID(Resource::RESOURCE_TYPE _rsc);
	inline static std::unordered_map<Resource::RESOURCE_TYPE, unsigned> m_nextID {};
	
	
	using RESOURCE_TYPE = Resource::RESOURCE_TYPE;
	
	std::vector<std::filesystem::path> m_assetPaths;
	std::unordered_map<std::string, RESOURCE_TYPE> m_fileExtensions;



	// resource pool identifiers

	//std::unordered_map<RESOURCE_TYPE, std::vector<std::shared_ptr<Resource>>> m_resourcePool;
	std::unordered_map<RES_ID, std::shared_ptr<Resource>> m_resourcePoolIDLookup;
	std::unordered_map<std::string, std::shared_ptr<Resource>> m_resourcePoolNameLookup;
};




/*

	res manager
	- manages assets used by 



*/