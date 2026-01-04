#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


/*!
	@brief a class designed to manage resources used in the application 
*/

struct ResourceIdentifier {
	const RES_ID m_resourceID;
	const std::string m_resourceName;
};

class ResourceManager {

private:

public:
	ResourceManager();
public:
	
	void Init();
	
	void ScanResourcesInPath(std::filesystem::path _filePath, bool _recursive = true);
	
	// optional path.
	ResourceIdentifier AddResource(std::shared_ptr<BaseResource> _resource, std::filesystem::path _path);
	void RemoveResource(RES_ID _id); 
	void RemoveResource(std::string _name); 


	std::shared_ptr<BaseResource> GetResource(RES_ID _id);
	std::shared_ptr<BaseResource> GetResource(std::string _name);

	std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>& GetResourcePool();
	const std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>& GetResourcePool() const;

	

	// registration
	void RegisterFileExtension(std::string _extension, RESTYPE_ID _type);
	void DeregisterFileExtension(std::string _extension);
	RESTYPE_ID GetResourceType(std::string _extension) const;
	
	// file packing
	void PackResources();


public:
	template <std::derived_from<BaseResource> T>
	void RegisterResourceType();

public:
	void LoadPaths();

	void LoadDefaultResources();


	void LoadResource(std::filesystem::path _filePath);
private:
	static RES_ID GenerateID(RESTYPE_ID _rsc);
	
	
	std::vector<std::filesystem::path>							m_assetPaths;
	std::unordered_map<std::string, RESTYPE_ID>					m_fileExtensions;
	inline static std::unordered_map<RESTYPE_ID, unsigned>		m_nextID {};



	// resource pool identifiers
	
	// primary storage.
	



	std::unordered_map<std::string, RES_ID>						m_resourceNameToID;
	std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>	m_resourcePoolIDLookup;



	// secondary identifiers.
	std::unordered_map<unsigned, std::vector<RES_ID>>			m_resourceTypeManifest;


};

#include <arch/resources/res_resourceManagerDEF.hpp>


/*

	res manager
	- manages assets used by 



*/