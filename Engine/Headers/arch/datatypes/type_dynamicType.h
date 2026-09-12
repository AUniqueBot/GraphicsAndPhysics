#pragma once
#include <arch/common/properties.h>



class IDynamicType {
public:
    virtual ~IDynamicType() = default;
    virtual PropertyMD::Property& GetProperty(void* _value) = 0;
    bool m_propertyMetaDirty    { true };
};

template <typename T>
class DynamicType : public IDynamicType {

};