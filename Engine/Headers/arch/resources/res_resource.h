#pragma once
#include <pch.h>

using ResourceID = size_t;

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

	using RES_ID = unsigned long;
	static const RES_ID RES_ID_INVALID{ 0 };
public:
	Resource(RESOURCE_TYPE _type);

	void LoadAsset();
	void UnloadAsset();
	void IsAssetLoaded() const;
private:


	RESOURCE_TYPE m_resType				{ _COUNT };
	bool m_isLoaded						{ false };
	unsigned m_referenceCount			{};
	std::filesystem::path m_pathToAsset	{}; // path to asset if any.
	ResourceID m_resourceId				{};
};