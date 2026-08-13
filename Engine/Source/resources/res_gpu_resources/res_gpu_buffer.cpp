#include <arch/resources/res_gpu_resources/res_gpu_buffer.h>


void GPU_Buffer::Create() {
	glCreateBuffers(1, &m_handle.Get());
    m_isCreated = (GLuint)m_handle != GPUConstants::C_INVALID_HANDLE;
}

void GPU_Buffer::Destroy() {
    if ((GLuint)m_handle) {
        glDeleteBuffers(1, &m_handle.Get());
        m_handle.Get() = 0;
        m_isCreated = false;
    }
}

void GPU_Buffer::Allocate(size_t _size, GLenum _usage) {
	glNamedBufferData((GLuint)m_handle, _size, nullptr, _usage);
    m_size = _size;
}


void GPU_Buffer::Upload(const void* _data, size_t _offset) {
    glNamedBufferSubData(
        (GLuint)m_handle,
        static_cast<GLintptr>(_offset),
        static_cast<GLsizeiptr>(m_size),
        _data
    );
}

