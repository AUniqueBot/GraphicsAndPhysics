#pragma once
#include <arch/resources/res_resource.h>


struct ResourceIdentifier {
	RES_ID m_resourceId;
	RESTYPE_ID m_resourceTypeId;
	std::string m_resourceName;
	ResourceManager* m_resourceManager;

	bool operator==(const ResourceIdentifier& _resId) const;

	std::shared_ptr<BaseResource> GetResource();

	std::shared_ptr<const BaseResource> GetResource() const;
};
