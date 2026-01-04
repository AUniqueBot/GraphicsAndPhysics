#pragma once
#include <pch.h>
/*
	Goals -> management of resources


*/


using RES_ID = unsigned long;
using RESTYPE_ID = uint32_t;

struct ResourceTypeMetadata {
	const std::string m_name;
	const size_t m_resourceTypeID;
	const type_info m_typeInfo;

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
	RESTYPE_ID m_resType			{ 0 };	// 0 reserved as invalid.
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