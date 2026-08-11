#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/resources/res_resourceManager.h>

class SpecializedResourceManager {
public:
	SpecializedResourceManager(ResourceManager& _manager) : m_resourceManager{_manager} {}
	
public:
	virtual void Init() {}
	virtual void Cleanup() {};

	// basic access
	bool Has(RES_ID _id) const;
	void Add(RES_ID _id);
	void Remove(RES_ID _id);

	// alias
	void SetResourceAlias(RES_ID _id, std::string _alias);
	void RemoveAliasForRes(RES_ID _id);
	RES_ID GetResIDFromAlias(std::string _alias) const;



protected:
	ResourceIdentifier RegisterResource(std::shared_ptr<BaseResource> _res);
	std::string GetAliasForRes(RES_ID _id) const;

protected:
	// current set contained.
	std::unordered_set<RES_ID> m_resourceIdPool;

	std::unordered_map<std::string, RES_ID> m_aliasToResId;
	std::unordered_map<RES_ID, std::string> m_resIdToAlias;

	ResourceManager& m_resourceManager;
};