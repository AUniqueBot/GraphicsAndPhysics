#pragma once
#include <pch.h>

template <typename T>
struct VertexAttributeTraits;


// ============================================================
// Scalar types
// ============================================================

template <>
struct VertexAttributeTraits<float> {
    using ScalarType = float;

    static constexpr GLenum GLType = GL_FLOAT;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(float);
};

template <>
struct VertexAttributeTraits<double> {
    using ScalarType = double;

    static constexpr GLenum GLType = GL_DOUBLE;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(double);
};


template <>
struct VertexAttributeTraits<int8_t> {
    using ScalarType = int8_t;

    static constexpr GLenum GLType = GL_BYTE;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(int8_t);
};

template <>
struct VertexAttributeTraits<uint8_t> {
    using ScalarType = uint8_t;

    static constexpr GLenum GLType = GL_UNSIGNED_BYTE;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(uint8_t);
};

template <>
struct VertexAttributeTraits<int16_t> {
    using ScalarType = int16_t;

    static constexpr GLenum GLType = GL_SHORT;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(int16_t);
};

template <>
struct VertexAttributeTraits<uint16_t> {
    using ScalarType = uint16_t;

    static constexpr GLenum GLType = GL_UNSIGNED_SHORT;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(uint16_t);
};

template <>
struct VertexAttributeTraits<int32_t> {
    using ScalarType = int32_t;

    static constexpr GLenum GLType = GL_INT;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(int32_t);
};

template <>
struct VertexAttributeTraits<uint32_t> {
    using ScalarType = uint32_t;

    static constexpr GLenum GLType = GL_UNSIGNED_INT;
    static constexpr size_t FeatureCount = 1;
    static constexpr size_t DatatypeSize = sizeof(uint32_t);
};


// ============================================================
// GLM vector types
// ============================================================

template <glm::length_t L, typename T, glm::qualifier Q>
struct VertexAttributeTraits<glm::vec<L, T, Q>> {
    using ScalarType = T;

    static constexpr GLenum GLType = VertexAttributeTraits<T>::GLType;
    static constexpr size_t FeatureCount = L;
    static constexpr size_t DatatypeSize = sizeof(T) * L;
};