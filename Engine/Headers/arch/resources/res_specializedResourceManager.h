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

	inline void SetResourceAlias(RES_ID _id, std::string _alias) {
		if (_alias.empty()) {
			LOG_INFO("Provide an alias");
			return;
		}
		if (!m_resourceIdPool.contains(_id)) {
			LOG_INFO("ID is not for this resource type.");
			return;
		}

		auto itr = m_aliasToResId.find(_alias);
		if (itr != m_aliasToResId.end() && !m_aliasToResId.empty()) {
			LOG_INFO("Overwriting an existing alias, \"" << _alias << "\". ID [" << itr->second << "] -> [" << _id << "]");
			RemoveAliasForRes(_id);
		}


		m_aliasToResId[_alias] = _id;
		m_resIdToAlias[_id] = _alias;
	}

	
	inline void RemoveAliasForRes(RES_ID _id) {
		std::string alias = GetAliasForRes(_id);
		if (alias.empty()) {
			LOG_INFO("No alias found for RES_ID: [" << _id << "]. Exiting.");
			return;
		}
		m_aliasToResId.erase(alias);
		m_resIdToAlias.erase(_id);
	}

	inline RES_ID GetResIDFromAlias(std::string _alias) {
		auto itr = m_aliasToResId.find(_alias);
		return itr == m_aliasToResId.end() ? BaseResource::C_RES_ID_INVALID : itr->second;
	}

private:
	inline std::string GetAliasForRes(RES_ID _id) {
		auto itr = m_resIdToAlias.find(_id);
		return itr == m_resIdToAlias.end() ? "" : itr->second;
	}

protected:
	// current set contained.
	std::unordered_set<RES_ID> m_resourceIdPool;

	std::unordered_map<std::string, RES_ID> m_aliasToResId;
	std::unordered_map<RES_ID, std::string> m_resIdToAlias;

	ResourceManager& m_resourceManager;
};