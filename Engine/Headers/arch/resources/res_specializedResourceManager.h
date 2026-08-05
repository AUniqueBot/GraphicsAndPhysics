#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/resources/res_resourceManager.h>


class SpecializedResourceManager {
public:
	SpecializedResourceManager(ResourceManager& _manager) : m_resourceManager{_manager} {}
	
	virtual void Init() {}
	virtual void Cleanup() {};
	inline bool Has(RES_ID _id) const { return m_resourceIdPool.contains(_id); };
	inline void Add(RES_ID _id) { m_resourceIdPool.insert(_id); };
	inline void Remove(RES_ID _id) { m_resourceIdPool.erase(_id); };

protected:
	std::unordered_set<RES_ID> m_resourceIdPool;
	ResourceManager& m_resourceManager;
};