#pragma once
#include <arch/resources/res_resourceManager.h>
#include <util/util_serialisation.h>


template<std::derived_from<BaseResource> T>
inline ResourceIdentifier ResourceManager::AddResource(std::shared_ptr<T> _resource, std::filesystem::path _path) {
	ResourceTypeMetadata rtm	{ RegisterResourceType<T>() };
	return AddResourceInternal(_resource, _resource->ResourceType(),_path);
}

template <std::derived_from<BaseResource> T>
ResourceTypeMetadata ResourceManager::RegisterResourceType() {
	const std::type_index typeInfo	{ typeid(T) };
	const std::string typeName		{ Reflection::TypeName<T>() };
	const RESTYPE_ID id				{ Resource<T>::GetResourceTypeID() };
	// storage into a map.

	const ResourceTypeMetadata data { typeName, id, typeInfo };
	m_resourceTypeMetadata.insert({ id, data });
	return m_resourceTypeMetadata.at(id);
}


