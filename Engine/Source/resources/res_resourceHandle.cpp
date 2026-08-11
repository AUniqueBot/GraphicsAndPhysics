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

std::shared_ptr<BaseResource> ResourceHandle::operator->() {
	if (!m_resourceIdentifier.has_value()) return nullptr;
	ResourceIdentifier& resIdr = *m_resourceIdentifier;
	ResourceManager& resMgr = *resIdr.m_resourceManager;
	return resMgr.GetResource(resIdr.m_resourceId);
}

ResourceIdentifier& ResourceHandle::operator*() {
	assert(m_resourceIdentifier.has_value() && "Identifier does not have member .");
	return *m_resourceIdentifier;
}

RES_ID ResourceHandle::GetResourceID() const {
	return HandleIsValid() ? 
		m_resourceIdentifier->m_resourceId : BaseResource::C_RES_ID_INVALID;
}

const ResourceIdentifier& ResourceHandle::GetResourceIdentifier() const {
	return *m_resourceIdentifier;
}

void ResourceHandle::SetName(const std::string& _name) {
	auto res = GetResource<BaseResource>();
	if (res) res->Name(_name);
}

std::string ResourceHandle::GetName() const {
	auto res = GetResource<BaseResource>();
	return res ? res->Name() : "__INVALID__RESOURCE__";
}

std::shared_ptr<BaseResource> ResourceHandle::GetBaseResource() {
	return m_resourceIdentifier->GetResource();
}

std::shared_ptr<const BaseResource> ResourceHandle::GetBaseResource() const {
	return m_resourceIdentifier->GetResource();
}



