#pragma once
#include <pch.h>
#include <nameof.hpp>
#include <arch/common/properties.h>
#include <typeindex>

using DynamicTypeID = std::type_index;
struct DynamicTypeInfo {
    DynamicTypeID m_id;
    std::string_view m_name;
};



class DynamicTypeRegistry {
public:
    template<typename T>
    inline void Register() {
        std::type_index type = typeid(T);
        DynamicTypeInfo info{type, nameof::nameof_short_type<T>() };
        m_types.insert(type, info);
    }

    template<typename T>
    inline const DynamicTypeInfo& Get() const {
        std::type_index type = typeid(T);
        return m_types.at(type);
    }

private:
    std::unordered_map<DynamicTypeID, DynamicTypeInfo> m_types;
};