#include "res_resource.h"

Resource::Resource(RESOURCE_TYPE _type) 
	: m_resType{ _type }
{

}

void Resource::LoadAsset() {
	if (m_isLoaded) return;
	m_isLoaded = true;
}

void Resource::UnloadAsset() {
	if (!m_isLoaded) return;
	m_isLoaded = false;
}

bool Resource::IsAssetLoaded() const {
	return m_isLoaded;
}

RES_ID Resource::ResourceID() const {
	return m_resourceId;
}
void Resource::ResourceID(RES_ID _newId) {
	m_resourceId = _newId;
}


std::filesystem::path Resource::ResourcePath() const {
	return m_pathToAsset;
}

void Resource::ResourcePath(std::filesystem::path _path) {
	m_pathToAsset = _path;
}
