#pragma once
#include <pch.h>
#include <arch/resources/res_resourceIdentifier.h>


using ResourceIdentifierArg = std::optional<ResourceIdentifier>;
struct ResourceHandle {
	ResourceHandle(ResourceIdentifierArg _id = std::nullopt);
	bool operator==(const ResourceHandle& _resHandle) const;

	bool HandleIsValid() const;

	std::shared_ptr<BaseResource> operator->();

	ResourceIdentifier& operator*();

	RES_ID GetResourceID() const;

	std::optional<ResourceIdentifier> GetResourceIdentifier();
	const ResourceIdentifier& GetResourceIdentifier() const;

	void SetName(const std::string& _name);
	inline std::string GetName() const;

protected:
	template <std::derived_from<BaseResource> T>
	inline std::shared_ptr<T> GetResource() {
		return std::static_pointer_cast<T>(GetBaseResource());
	}

	template <std::derived_from<BaseResource> T>
	inline std::shared_ptr<const T> GetResource() const {
		return std::static_pointer_cast<const T>(GetBaseResource());
	}

private:
	std::shared_ptr<BaseResource> GetBaseResource();
	std::shared_ptr<const BaseResource> GetBaseResource() const;

private:
	std::optional<ResourceIdentifier> m_resourceIdentifier;

};