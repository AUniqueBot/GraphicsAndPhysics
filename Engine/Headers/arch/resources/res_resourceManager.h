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
	void AddResource(std::shared_ptr<Resource>  _resource, std::filesystem::path _path = ""); 
	void RemoveResource(RES_ID _id); 
	void GetResource(RES_ID _id);



	

	// registration
	void RegisterFileExtension(std::string _extension, Resource::RESOURCE_TYPE _type);
	void DeregisterFileExtension(std::string _extension);
	
	// file packing
	void PackResources();

public:
	void LoadPaths();


private:
	static RES_ID GenerateID(Resource::RESOURCE_TYPE _rsc);
	inline static std::unordered_map<Resource::RESOURCE_TYPE, unsigned> m_nextID {};
	
	
	using RESOURCE_TYPE = Resource::RESOURCE_TYPE;
	
	std::vector<std::filesystem::path> m_assetPaths;
	std::unordered_map<std::string, RESOURCE_TYPE> m_fileExtensions;
	std::unordered_map<RESOURCE_TYPE, std::vector<std::shared_ptr<Resource>>> m_resourcePool;
};




/*

	res manager
	- manages assets used by 



*/