#pragma once


class Resource {
private:
public:
	enum RESOURCE_TYPE {
		// Graphics
		TEXTURE,
		MATERIAL,
		MESH,
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
	Resource(RESOURCE_TYPE _type = _COUNT) : m_resType { _type } {}


private:

	// bit 1 - loaded/unloaded
	// do I want bits for id recognition?
	// bwoah fine.

	RESOURCE_TYPE m_resType	{ _COUNT };
};