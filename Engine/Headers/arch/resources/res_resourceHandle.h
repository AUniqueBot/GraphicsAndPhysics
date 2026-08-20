#pragma once
#include <pch.h>
#include <arch/resources/res_resourceIdentifier.h>


using ResourceIdentifierArg = std::optional<ResourceIdentifier>;
struct ResourceHandle {
	ResourceHandle(ResourceIdentifierArg _id = std::nullopt);
	bool operator==(const ResourceHandle& _resHandle) const;

	bool HandleIsValid() const;

	RES_ID GetResourceID() const;

	std::optional<ResourceIdentifier> GetResourceIdentifier();
	const ResourceIdentifier& GetResourceIdentifier() const;

	void SetName(const std::string& _name);
	inline std::string GetName() const;

	std::shared_ptr<BaseResource> GetBaseResource();
	std::shared_ptr<const BaseResource> GetBaseResource() const;

protected:
    template <std::derived_from<BaseResource>T>
    std::shared_ptr<T> GetResourceT() {
        return static_pointer_cast<T>(GetBaseResource());
    }
    template <std::derived_from<BaseResource>T>
    std::shared_ptr<const T> GetResourceT() const {
        return static_pointer_cast<const T>(GetBaseResource());
    }

private:
	std::optional<ResourceIdentifier> m_resourceIdentifier;

};


template <std::derived_from<BaseResource>T>
struct ResourceHandleT : public ResourceHandle {
    using ResourceHandle::ResourceHandle;
    ResourceHandleT(ResourceIdentifierArg _arg = std::nullopt) : ResourceHandle(_arg) {};
    std::shared_ptr<T> Get() {
        return GetResourceT<T>();
    }

    std::shared_ptr<const T> Get() const {
        return GetResourceT<const T>();
    }


};

template <
    std::derived_from<BaseResource> Base,
    std::derived_from<Base> Derived
>
struct ResourceHandleDerivedT : public ResourceHandleT<Base> {
    using BaseHandle = ResourceHandleT<Base>;

    ResourceHandleDerivedT(ResourceIdentifierArg id = std::nullopt)
        : BaseHandle(id) {
    }

    Derived* operator->() {
        return Get().get();
    }

    const Derived* operator->() const {
        return Get().get();
    }

    std::shared_ptr<Derived> Get() {
        return ResourceHandle::GetResourceT<Derived>();
    }

    std::shared_ptr<const Derived> Get() const {
        return ResourceHandle::GetResourceT<const Derived>();
    }
};