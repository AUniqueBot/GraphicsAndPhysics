#include <arch/resources/res_specializedResourceManager.h>
#include <arch/resources/res_resourceIdentifier.h>

bool SpecializedResourceManager::Has(RES_ID _id) const { 
	return m_resourceIdPool.contains(_id); 
};
void SpecializedResourceManager::Add(RES_ID _id) { 
	m_resourceIdPool.insert(_id); 
};
void SpecializedResourceManager::Remove(RES_ID _id) { 
	m_resourceIdPool.erase(_id); 
};

void SpecializedResourceManager::SetResourceAlias(RES_ID _id, std::string _alias) {
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


void SpecializedResourceManager::RemoveAliasForRes(RES_ID _id) {
	std::string alias = GetAliasForRes(_id);
	if (alias.empty()) {
		LOG_INFO("No alias found for RES_ID: [" << _id << "]. Exiting.");
		return;
	}
	m_aliasToResId.erase(alias);
	m_resIdToAlias.erase(_id);
}

RES_ID SpecializedResourceManager::GetResIDFromAlias(std::string _alias) const {
	auto itr = m_aliasToResId.find(_alias);
	return itr == m_aliasToResId.end() ? BaseResource::C_RES_ID_INVALID : itr->second;
}




ResourceIdentifier SpecializedResourceManager::RegisterResource(std::shared_ptr<BaseResource> _res) {
	ResourceIdentifier idr = m_resourceManager.AddInternalResource(_res);
	Add(idr.m_resourceId);
	return idr;
}

std::string SpecializedResourceManager::GetAliasForRes(RES_ID _id) const {
	auto itr = m_resIdToAlias.find(_id);
	return itr == m_resIdToAlias.end() ? "" : itr->second;
}
