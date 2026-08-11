#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_resourceIdentifier.h>

bool ResourceIdentifier::operator==(const ResourceIdentifier& _resId) const {
	return
		m_resourceManager == _resId.m_resourceManager &&
		m_resourceId == _resId.m_resourceId &&
		m_resourceTypeId == m_resourceTypeId
		; // the name doesn't matter as much.
}

std::shared_ptr<BaseResource> ResourceIdentifier::GetResource() {
	return std::static_pointer_cast<BaseResource>(m_resourceManager->GetResource(m_resourceId));
}

std::shared_ptr<const BaseResource> ResourceIdentifier::GetResource() const {
	return std::static_pointer_cast<const BaseResource>(m_resourceManager->GetResource(m_resourceId));
}