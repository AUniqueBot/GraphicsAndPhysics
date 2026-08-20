#pragma once
#include <arch/resources/res_resource.h>


struct ResourceIdentifier {
	RES_ID m_resourceId						{ ResourceConstants::C_RES_INVALID_ID };
	RESTYPE_ID m_resourceTypeId				{ };
	ResourceManager* m_resourceManager		{ nullptr };
	std::string m_resourceName;

	bool operator==(const ResourceIdentifier& _resId) const;

	std::shared_ptr<BaseResource> GetResource();

	std::shared_ptr<const BaseResource> GetResource() const;
};
