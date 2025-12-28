#pragma once
#include <pch.h>
/*
	Goals -> management of resources


*/


using RES_ID = unsigned long;

class Resource {
private:
public:
	enum RESOURCE_TYPE {
		// - GPU assets -
		TEXTURE, 
		MATERIAL,
		MESH,
		SHADER,
		// -- WIP --
		ANIMATION,
		// Other
		SCRIPT, // to be using mono c.
		AUDIO,
		PREFAB,
		SCENE,
		_COUNT
	};

	
	static const RES_ID RES_ID_INVALID { 0 };
public:
	Resource(RESOURCE_TYPE _type);
	RESOURCE_TYPE ResourceType() const	{ return m_resType; }


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
	RESOURCE_TYPE m_resType				{ _COUNT };
	unsigned m_referenceCount			{};
	bool m_isLoaded						{ false };
};