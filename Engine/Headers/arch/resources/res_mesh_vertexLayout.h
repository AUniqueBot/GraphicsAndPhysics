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
    virtual ~VertexAttributeDatabase() = default;
    virtual size_t ElementCount() const = 0;
    virtual size_t FeatureCount() const = 0; // vec3 = 3, vec2 = 2
};

template<typename T>
struct VertexAttributeData : VertexAttributeDatabase {
    std::vector<T> m_data;

    VertexAttributeData() = default;
    VertexAttributeData(std::vector<T>&& data) : m_data(std::move(data)) {}

    size_t ElementCount() const override { return m_data.size(); }
    size_t FeatureCount() const override { return sizeof(T) / sizeof(float); } // assumes packed floats
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