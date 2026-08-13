#pragma once
#include <pch.h>


namespace GPUConstants {
    constexpr GLuint C_INVALID_HANDLE = 0;
}

class GPUHandle {
public:
    GPUHandle() = default;
    explicit GPUHandle(GLuint _handle) : m_handle(_handle) {}
    GPUHandle(const GPUHandle& _handle) = default;

    operator GLuint&()                  { return m_handle; }
    operator GLuint() const             { return m_handle; }

    inline const GLuint& Get() const    { return m_handle; }
    inline bool IsValid() const         { return m_handle != GPUConstants::C_INVALID_HANDLE; }


    inline GLuint& Get()                { return m_handle; }
private:    
    GLuint m_handle                     { GPUConstants::C_INVALID_HANDLE };
};