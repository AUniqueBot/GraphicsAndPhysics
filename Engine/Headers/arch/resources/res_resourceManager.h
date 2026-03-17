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


template <std::derived_from<BaseResource> T>
struct ResourceSlot {


	void GetPointerHandle() {
		// returns a class
	}

	std::unique_ptr<T> m_resourcePointer;
};


template <std::derived_from<BaseResource> T>
struct ResourceHandler {
	ResourceHandler() {

	}
	~ResourceHandler() {

	}

	
};


class ResourceManager {

private:

public:
	ResourceManager();
public:
	
	void Init();
	
	void ScanResourcesInPath(std::filesystem::path _filePath, bool _recursive = true);
	

	// internal thing
	template <std::derived_from<BaseResource> T>
	ResourceIdentifier AddResource(std::shared_ptr<T> _resource, std::filesystem::path _path);
	

	void RemoveResource(RES_ID _id); 
	void RemoveResource(std::string _name); 


	std::shared_ptr<BaseResource> GetResource(RES_ID _id);
	std::shared_ptr<BaseResource> GetResource(std::string _name);

	std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>& GetResourcePool();
	const std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>& GetResourcePool() const;

	const std::vector<RES_ID>& GetResourcePoolManifest(RESTYPE_ID _typeId) const;
	
	
	
	const ResourceTypeMetadata& GetResourceTypeMetadata(RESTYPE_ID _typeId) const {
		return m_resourceTypeMetadata.at(_typeId);
	}

	template <std::derived_from<BaseResource>>
	void GetResourceTypeMetadata()const {

	}


	// registration
	void RegisterFileExtension(std::string _extension, RESTYPE_ID _type);
	void DeregisterFileExtension(std::string _extension);
	RESTYPE_ID GetResourceType(std::string _extension) const;
	
	// file packing
	void PackResources();


public:
	template <std::derived_from<BaseResource> T>
	ResourceTypeMetadata RegisterResourceType();

public:
	void LoadPaths();

	void LoadDefaultResources();


	void LoadResource(std::filesystem::path _filePath);
private:
	// optional path.
	ResourceIdentifier AddResourceInternal(
		std::shared_ptr<BaseResource> _resource, 
		RESTYPE_ID _type, 
		std::filesystem::path _path
	);

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
	std::unordered_map<RESTYPE_ID, ResourceTypeMetadata>		m_resourceTypeMetadata;
	std::unordered_map<RESTYPE_ID, std::vector<RES_ID>>			m_resourceTypeManifest;

	
};






#include <arch/resources/res_resourceManagerDEF.hpp>


/*

	res manager
	- manages assets used by 



*/