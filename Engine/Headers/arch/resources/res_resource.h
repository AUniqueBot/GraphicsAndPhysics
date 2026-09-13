#pragma once
#include <pch.h>
#include <typeindex>
#include <arch/common/inspectable.h>
#include <arch/resources/res_resourceTypeMetadata.h>
#include <nameof.hpp>
//#include <arch/common/properties.h>
/*
	Goals -> management of resources


*/


using RES_ID = uint64_t;		// id of the resource (to be replaced with GUID one day)



namespace ResourceConstants {
	inline constexpr const RES_ID	  C_RES_INVALID_ID		{ 0 };
};


inline RES_ID GenerateResourceID() {
	static std::atomic<RES_ID> next{ 1 }; // 0 reserved = invalid
	return next++;
}


class BaseResource: public Inspectable {
public:

public:
	BaseResource(RESTYPE_ID _type);
	RESTYPE_ID ResourceType() const	{ return m_resType; }
	inline virtual std::string ResourceTypeName() { return "RESOURCE"; };

	std::string Name() const;
	void Name(std::string _name);


	virtual void LoadAsset();
	virtual void LoadAsset(std::filesystem::path _path);
	virtual void UnloadAsset();
	bool IsAssetLoaded() const;

	void ResourceID(RES_ID _newId);
	RES_ID ResourceID() const;
	

	std::filesystem::path ResourcePath() const;
	void ResourcePath(std::filesystem::path _path);


	inline virtual void Destroy() {};


protected:
	friend class ResourceManager;
	std::string m_name{};
	std::filesystem::path m_pathToAsset	{}; // path to asset if any.

private:
	/*
		Serialization rules:
			if has alias - use alias
			else if has path - use path (guid)
			else use constructed
	*/

	//
	RES_ID m_resourceId					{ ResourceConstants::C_RES_INVALID_ID };
	RESTYPE_ID m_resType				{ ResourceConstants::C_RESTYPE_INVALID_ID };	// 0 reserved as invalid.
	unsigned m_referenceCount			{};
	bool m_isLoaded						{ false };
	bool m_generated					{ true }; // unneeded if the resource id has an alias.

};


#ifndef RESOURCE_TYPENAMEOVERRIDE(T)
#define RESOURCE_TYPENAMEOVERRIDE(T) \
	public: \
	inline std::string ResourceTypeName() override { return #T; }
#endif

// ----------------------------------------------------------------------------------
// wrapper class for inheritance
// ----------------------------------------------------------------------------------


inline RESTYPE_ID GenerateResourceTypeID() {
	static std::atomic<RESTYPE_ID> next{ 1 }; // 0 reserved = invalid
	return next++;
}


template <typename T>
class Resource : public BaseResource {

protected:
	Resource() : BaseResource(GetResourceTypeID()) {};
	
public :
	const static RESTYPE_ID GetResourceTypeID() {
		static const RESTYPE_ID id = GenerateResourceTypeID();
		return id;
	}


	inline virtual std::vector<PropertyMD::Property>& GetProperties() {
		static std::vector< PropertyMD::Property> prop;
		return prop;
	}
};


template<typename T>
concept ResourceType = std::derived_from<T, Resource<T>>;


