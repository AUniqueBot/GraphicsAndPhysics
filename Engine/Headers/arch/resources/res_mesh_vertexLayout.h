#pragma once
#include <pch.h>
#include <arch/resources/res_mesh/res_mesh_vertexAttributeTraits.h>
namespace VAOConstants {
    inline constexpr const char* C_VAO_STATIC_MESH = "C_VAO_STATIC_MESH";
    inline constexpr const char* C_VAO_SKINNED_MESH = "C_VAO_SKINNED_MESH";
}

struct VertexAttributeDesc {
    std::string m_name;
    GLenum m_type;
    uint32_t m_featureCount;
    bool m_normalized;
};

struct VertexLayout {
    std::string name;
    std::vector<VertexAttributeDesc> attributes;
};


struct VertexAttributeDatabase {
    VertexAttributeDatabase() = default;
    virtual ~VertexAttributeDatabase() = default;
    virtual size_t ElementCount() const = 0;
    virtual size_t FeatureCount() const = 0; 
    virtual size_t DatatypeSize() const = 0;
    virtual size_t DataSize() const = 0;
    virtual const void* Data() const = 0;
};

template<typename T>
struct VertexAttributeData : VertexAttributeDatabase {
    std::vector<T> m_data;

    VertexAttributeData() = default;
    VertexAttributeData(std::vector<T>&& data) : m_data(std::move(data)) {}

    inline size_t ElementCount() const override { return m_data.size(); }
    inline size_t FeatureCount() const override { return VertexAttributeTraits<T>::FeatureCount; } // assumes packed floats
    inline size_t DatatypeSize() const override { return VertexAttributeTraits<T>::DatatypeSize; }
    inline size_t DataSize() const override     { return m_data.size() * VertexAttributeTraits<T>::DatatypeSize; }
    inline const void* Data() const override    { return m_data.data(); }

private:
};


namespace VertexLayouts {
    inline const VertexLayout C_STATIC_MESH = {
        VAOConstants::C_VAO_STATIC_MESH,
        {
            { "position", GL_FLOAT, 3, false },
            { "normal",   GL_FLOAT, 3, false },
            { "uv0",      GL_FLOAT, 2, false },
        }
    };
    inline const VertexLayout C_SKINNED_MESH = {
        VAOConstants::C_VAO_SKINNED_MESH,
        {
            { "position",     GL_FLOAT, 3, false },
            { "normal",       GL_FLOAT, 3, false },
            { "uv0",          GL_FLOAT, 2, false },
            { "boneid",       GL_UNSIGNED_INT, 4, false },
            { "boneweights",  GL_FLOAT, 4, false },
        }
    };

}