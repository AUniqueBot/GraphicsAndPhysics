#pragma once
#include <pch.h>


namespace GPUConstants {
    inline constexpr uint32_t C_INVALID_HANDLE = 0;
}

class GPUHandle {
public:
    GPUHandle() = default;
    explicit GPUHandle(GLuint _handle) : m_handle(_handle) {}
    GPUHandle(const GPUHandle& _handle) = default;

    operator uint32_t&()                  { return m_handle; }
    operator uint32_t() const             { return m_handle; }

    inline const uint32_t& Get() const    { return m_handle; }
    inline bool IsValid() const         { return m_handle != GPUConstants::C_INVALID_HANDLE; }


    inline uint32_t& Get()                { return m_handle; }
private:    
    uint32_t m_handle                     { GPUConstants::C_INVALID_HANDLE };
};