#pragma once
#include <arch/common/properties.h>

class Inspectable {


public:
	virtual std::vector<PropertyMD::Property>& GetProperties() {
		static std::vector<PropertyMD::Property> props{};
		return props;
	};
};


template<typename T>
struct InspectTraits {
    static void* GetRaw(T& value) {
        return &value;
    }

    static Inspectable* GetInspectable(T& value) {
        if constexpr (std::is_base_of_v<Inspectable, T>) {
            return &value;
        }
        return nullptr;
    }
};

// Pointer specialization
template<typename T>
struct InspectTraits<T*> {
    static void* GetRaw(T* value) {
        return value;
    }

    static Inspectable* GetInspectable(T* value) {
        if constexpr (std::is_base_of_v<Inspectable, T>) {
            return value;
        }
        return nullptr;
    }
};

// shared_ptr specialization
template<typename T>
struct InspectTraits<std::shared_ptr<T>> {
    static void* GetRaw(std::shared_ptr<T>& value) {
        return value.get();
    }

    static Inspectable* GetInspectable(std::shared_ptr<T>& value) {
        if constexpr (std::is_base_of_v<Inspectable, T>) {
            return value.get();
        }
        return nullptr;
    }
};

#define INSPECTABLE_DECLAREPROPS(TYPE) \
public:\
	inline std::vector<PropertyMD::Property>& GetProperties() override { return TYPE::GetProps(); }; \
private:\
	static std::vector<PropertyMD::Property>& GetProps();