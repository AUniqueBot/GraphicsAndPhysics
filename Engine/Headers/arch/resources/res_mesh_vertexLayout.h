#pragma once
#include <pch.h>



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
    inline size_t FeatureCount() const override { return sizeof(T) / sizeof(float); } // assumes packed floats
    inline size_t DatatypeSize() const override { return sizeof(T); }
    inline size_t DataSize() const override     { return m_data.size() * sizeof(T); }
    inline const void* Data() const override    { return m_data.data(); }
};


namespace VertexLayouts {
    inline const VertexLayout C_STATIC_MESH = {
        "StaticMesh",
        {
            { "position", GL_FLOAT, 3, false },
            { "normal",   GL_FLOAT, 3, false },
            { "uv0",      GL_FLOAT, 2, false },
        }
    };
    inline const VertexLayout C_SKINNED_MESH = {
        "SkinnedMesh",
        {
            { "position",     GL_FLOAT, 3, false },
            { "normal",       GL_FLOAT, 3, false },
            { "uv0",          GL_FLOAT, 2, false },
            { "boneid",       GL_UNSIGNED_INT, 4, false },
            { "boneweights",  GL_FLOAT, 4, false },
        }
    };

}