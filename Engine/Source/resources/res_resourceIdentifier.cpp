#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_resourceIdentifier.h>

std::shared_ptr<BaseResource> ResourceIdentifier::GetResource() {
	return std::static_pointer_cast<BaseResource>(m_resourceManager->GetResource(m_resourceId));
}

std::shared_ptr<const BaseResource> ResourceIdentifier::GetResource() const {
	return std::static_pointer_cast<const BaseResource>(m_resourceManager->GetResource(m_resourceId));
}