#pragma once
#include <pch.h>
#include <typeindex>
/*
	Goals -> management of resources


*/


using RES_ID = unsigned long;	//id of the resource (to be replaced with GUID one day)
using RESTYPE_ID = uint32_t;	// id of the TYPE of resource

struct ResourceTypeMetadata {
public:
	using RESTYPE_INFO = std::type_index;
public:
	ResourceTypeMetadata(
		std::string _typeName,
		RESTYPE_ID _typeId,
		std::type_index _typeInfo
	) :
		m_name				{ _typeName },
		m_resourceTypeID	{ _typeId },
		m_typeInfo			{ _typeInfo }

	{

	}

	const std::string& GetName() const { return m_name; }
	const RESTYPE_ID& GetResourceTypeID() const { return m_resourceTypeID; }
	const RESTYPE_INFO& GetTypeInfo() const { return m_typeInfo; }

private:
	std::string m_name;
	RESTYPE_ID m_resourceTypeID;
	std::type_index m_typeInfo;
};

class BaseResource {
private:
public:

	static const RES_ID RES_ID_INVALID { 0 };
public:
	BaseResource(RESTYPE_ID _type);
	RESTYPE_ID ResourceType() const	{ return m_resType; }


	virtual void LoadAsset();
	virtual void UnloadAsset();
	bool IsAssetLoaded() const;

	RES_ID ResourceID() const;
	

	std::filesystem::path ResourcePath() const;
	void ResourcePath(std::filesystem::path _path);

private:
	friend class ResourceManager;
	void ResourceID(RES_ID _newId);


	// Res ID structure (total 64 bits)
	// 8 Bits - 3 Digits for asset type
	// 24 bits - something idk 



protected:
	std::filesystem::path m_pathToAsset	{}; // path to asset if any.

private:
	RES_ID m_resourceId					{ RES_ID_INVALID };
	RESTYPE_ID m_resType				{ 0 };	// 0 reserved as invalid.
	unsigned m_referenceCount			{};
	bool m_isLoaded						{ false };
};

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
};


class ResourceContainer {
public:
	ResourceContainer();
private:
	std::unique_ptr<BaseResource> m_resPtr;
};