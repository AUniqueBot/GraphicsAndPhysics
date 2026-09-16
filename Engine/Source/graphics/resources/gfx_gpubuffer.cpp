#include <graphics/resources/gfx_gpubuffer.h>


void GPUBuffer::Create() {
	glCreateBuffers(1, &m_handle.Get());
    m_isCreated = (GLuint)m_handle != GPUConstants::C_INVALID_HANDLE;
}

void GPUBuffer::Destroy() {
    if ((GLuint)m_handle) {
        glDeleteBuffers(1, &m_handle.Get());
        m_handle.Get() = 0;
        m_isCreated = false;
    }
}

void GPUBuffer::Allocate(size_t _size, GLenum _usage) {
    glNamedBufferStorage((GLuint)m_handle, _size, nullptr, _usage);
    m_size = _size;
}

void GPUBuffer::SetUsage(GLenum _usage) {
    m_usage = _usage;
}

void GPUBuffer::SetSize(size_t _size) {
    m_size = _size;
}

size_t GPUBuffer::GetSize() const {
    return m_size;
}

void GPUBuffer::CreateImpl() {

}

void GPUBuffer::DestroyImpl() {
    
}


void GPUBuffer::Upload(
    const void* _data, 
    size_t _size, 
    size_t _offset
) {
    assert(_size <= m_size && "provided upload size is larger than allocated size");
    glNamedBufferSubData(
        (GLuint)m_handle,
        static_cast<GLintptr>(_offset),
        static_cast<GLsizeiptr>(_size),
        _data
    );
}

