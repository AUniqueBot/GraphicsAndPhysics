#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


/*!
	@brief a class designed to manage resources used in the application 
*/






template <std::derived_from<BaseResource> T>
struct ResourceSlot {


	void GetPointerHandle() {
		// returns a class
	}

	std::unique_ptr<T> m_resourcePointer;
};


struct ResourceIdentifier;

class ResourceManager {

private:

public:

public:
	
	void Init();
	
	void ScanResourcesInPath(std::filesystem::path _filePath, bool _recursive = true);
	

	// internal thing
	template <std::derived_from<BaseResource> T>
	ResourceIdentifier AddExternalResource(std::shared_ptr<T> _resource, std::filesystem::path _path);


	template <std::derived_from<BaseResource> T>
	ResourceIdentifier AddInternalResource(std::shared_ptr<T> _resource);

	bool RegisterResourceKey(RES_ID _resId, std::string _name);
	

	void RemoveResource(RES_ID _id); 
	void RemoveResource(std::string _name); 


	std::shared_ptr<BaseResource> GetResource(RES_ID _id);
	std::shared_ptr<BaseResource> GetResource(std::string _name);
	std::shared_ptr<BaseResource> GetResource(ResourceIdentifier _id);


	std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>& GetResourcePool();
	const std::unordered_map<RES_ID, std::shared_ptr<BaseResource>>& GetResourcePool() const;

	const std::vector<RES_ID>& GetResourcePoolManifest(RESTYPE_ID _typeId) const;
	
	
	
	const ResourceTypeMetadata& GetResourceTypeMetadata(RESTYPE_ID _typeId) const {
		return m_resourceTypeMetadata.at(_typeId);
	}

	template <std::derived_from<BaseResource>>
	void GetResourceTypeMetadata() const {

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
	ResourceIdentifier AddExternalResourceInternal(
		std::shared_ptr<BaseResource> _resource, 
		RESTYPE_ID _type, 
		std::filesystem::path _path
	);

	ResourceIdentifier AddInternalResourceInternal(
		std::shared_ptr<BaseResource> _resource,
		RESTYPE_ID _type
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

struct ResourceIdentifier {
	RES_ID m_resourceId;
	RESTYPE_ID m_resourceTypeId;
	std::string m_resourceName;
	ResourceManager* m_resourceManager;

	template <std::derived_from<BaseResource> T>
	inline std::shared_ptr<T> GetResource() {
		return std::static_pointer_cast<T>(m_resourceManager->GetResource(m_resourceId));
	}

	template <std::derived_from<BaseResource> T>
	inline std::shared_ptr<const T> GetResource() const {
		return std::static_pointer_cast<const T>(m_resourceManager->GetResource(m_resourceId));
	}
};

using ResourceIdentifierArg = std::optional<ResourceIdentifier>;

struct ResourceHandle {
	
	inline std::optional<ResourceIdentifier> GetResourceIdentifier() {
		return m_resourceIdentifier;
	}
	inline ResourceHandle(ResourceIdentifierArg _id = std::nullopt) : 
		m_resourceIdentifier { _id } {
	};
	inline bool HandleIsValid() const { return m_resourceIdentifier.has_value() && m_resourceIdentifier->m_resourceManager != nullptr; };
	inline std::shared_ptr<BaseResource> operator->() {
		if (!m_resourceIdentifier.has_value()) return nullptr;
		ResourceIdentifier& resIdr = *m_resourceIdentifier;
		ResourceManager& resMgr = *resIdr.m_resourceManager;
		return resMgr.GetResource(resIdr.m_resourceId);
	}

	inline ResourceIdentifier& operator*() {
		assert(m_resourceIdentifier.has_value() && "Identifier does not have member .");
		return *m_resourceIdentifier;
	}

	inline RES_ID GetResourceID() const {
		return HandleIsValid() ? m_resourceIdentifier->m_resourceId : BaseResource::C_RES_ID_INVALID;
	}

	inline const ResourceIdentifier& GetResourceIdentifier() const {
		return *m_resourceIdentifier;
	}

protected:
	template <std::derived_from<BaseResource> T>
	inline std::shared_ptr<T> GetResource() {
		return std::static_pointer_cast<T>(m_resourceIdentifier->GetResource<T>());
	}

	template <std::derived_from<BaseResource> T>
	inline std::shared_ptr<const T> GetResource() const {
		return std::static_pointer_cast<const T>(m_resourceIdentifier->GetResource<T>());
	}

private:
	std::optional<ResourceIdentifier> m_resourceIdentifier;
	
};

#include <arch/resources/res_resourceManagerDEF.hpp>


/*

	res manager
	- manages assets used by 



*/