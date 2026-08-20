#include <arch/resources/res_resourceHandle.h>
#include <arch/resources/res_resourceManager.h>

ResourceHandle::ResourceHandle(ResourceIdentifierArg _id) : m_resourceIdentifier{ _id } {}
bool ResourceHandle::operator==(const ResourceHandle& _resHandle) const {
	
	return m_resourceIdentifier == _resHandle.m_resourceIdentifier;
};

std::optional<ResourceIdentifier> ResourceHandle::GetResourceIdentifier() {
	return m_resourceIdentifier;
}
bool ResourceHandle::HandleIsValid() const { 
	return 
		m_resourceIdentifier.has_value() && 
		m_resourceIdentifier->m_resourceManager != nullptr; 
};

RES_ID ResourceHandle::GetResourceID() const {
	return HandleIsValid() ? 
		m_resourceIdentifier->m_resourceId : ResourceConstants::C_RES_INVALID_ID;
}

const ResourceIdentifier& ResourceHandle::GetResourceIdentifier() const {
	return *m_resourceIdentifier;
}

void ResourceHandle::SetName(const std::string& _name) {
	auto res = GetBaseResource();
	if (res) res->Name(_name);
}

std::string ResourceHandle::GetName() const {
	auto res = GetBaseResource();
	return res ? res->Name() : "__INVALID__RESOURCE__";
}

std::shared_ptr<BaseResource> ResourceHandle::GetBaseResource() {
	return m_resourceIdentifier->GetResource();
}

std::shared_ptr<const BaseResource> ResourceHandle::GetBaseResource() const {
	return m_resourceIdentifier->GetResource();
}



