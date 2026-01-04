#pragma once
#include <arch/resources/res_resourceManager.h>



template <std::derived_from<BaseResource> T>
void ResourceManager::RegisterResourceType() {
	const type_info typeID = typeid(T);


}