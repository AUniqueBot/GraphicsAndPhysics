#pragma once
#include <pch.h>

#include <arch/resources/res_mesh/res_mesh_vertexAttributeTraits.h>
namespace VAOConstants {
    inline constexpr const char* C_VAO_DEFAULT_MESH = "C_VAO_DEFAULT_MESH";
}

namespace VertexAttributeConstants {
    inline constexpr const char* C_VTXATTR_POSITION = "position";
    inline constexpr const char* C_VTXATTR_NORMAL = "normal";
    inline constexpr const char* C_VTXATTR_TANGENT = "tangent";
    inline constexpr const char* C_VTXATTR_COLOR = "color";

    inline constexpr const char* C_VTXATTR_UV0 = "uv0";
    inline constexpr const char* C_VTXATTR_UV1 = "uv1";
    inline constexpr const char* C_VTXATTR_UV2 = "uv2";
    inline constexpr const char* C_VTXATTR_UV3 = "uv3";
    inline constexpr const char* C_VTXATTR_UV4 = "uv4";
    inline constexpr const char* C_VTXATTR_UV5 = "uv5";
    inline constexpr const char* C_VTXATTR_UV6 = "uv6";
    inline constexpr const char* C_VTXATTR_UV7 = "uv7";
    
    inline constexpr const char* C_VTXATTR_BONEWEIGHTS = "boneweights";
    inline constexpr const char* C_VTXATTR_BONEINDICES = "boneindices";

    inline constexpr size_t C_VTXATTR_MAX_VERTEX_BINDINGS = 100;
}



struct VertexAttributeDesc {
    std::string m_name;

    GLuint m_attributeSlot;
    GLuint m_bindingSlot;

    GLenum m_type;
    uint32_t m_featureCount;
    bool m_normalized;
    int m_offset = 0;


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
    virtual GLenum BaseDatatype() const = 0;
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
    inline GLenum BaseDatatype() const override { return VertexAttributeTraits<T>::GLType; }
    inline const void* Data() const override    { return m_data.data(); }

private:
};





namespace VertexLayouts {
    using namespace VertexAttributeConstants;


    inline const VertexLayout C_DEFAULT_MESH = {
        VAOConstants::C_VAO_DEFAULT_MESH,
        {
            { C_VTXATTR_POSITION,    0, 0, GL_FLOAT, 3, false },
            { C_VTXATTR_NORMAL,      1, 1, GL_FLOAT, 3, false },
            { C_VTXATTR_UV0,         2, 2, GL_FLOAT, 2, false },

            { C_VTXATTR_BONEINDICES, 3, 3, GL_UNSIGNED_INT, 4, false },
            { C_VTXATTR_BONEWEIGHTS, 4, 4, GL_FLOAT, 4, false },
        }
    };

}


class aiMesh;
VertexLayout GenerateVertexLayout(const aiMesh& _mesh);