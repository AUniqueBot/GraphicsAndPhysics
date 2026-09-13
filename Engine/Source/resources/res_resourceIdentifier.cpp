#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_resourceIdentifier.h>

bool ResourceIdentifier::operator==(const ResourceIdentifier& _resId) const {
	return
		m_resourceManager == _resId.m_resourceManager 
		&& m_resourceId == _resId.m_resourceId 
		//&& m_resourceTypeId == m_resourceTypeId
		; // the name doesn't matter as much.
}

std::shared_ptr<BaseResource> ResourceIdentifier::GetResource() {
	if (!IsValid()) return nullptr;
	return m_resourceManager->GetResource(m_resourceId);
}

std::shared_ptr<const BaseResource> ResourceIdentifier::GetResource() const {
	if (!IsValid()) return nullptr;
	return m_resourceManager->GetResource(m_resourceId);
}

bool ResourceIdentifier::IsValid() const {
	return
		m_resourceManager != nullptr &&
		m_resourceId != ResourceConstants::C_RES_INVALID_ID &&
		m_resourceTypeId != ResourceConstants::C_RESTYPE_INVALID_ID;

}