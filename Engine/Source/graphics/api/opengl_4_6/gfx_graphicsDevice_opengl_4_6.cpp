#include <graphics/api/opengl_4_6/gfx_graphicsDevice_opengl_4_6.h>

GPUBuffer OpenGL4_6_GraphicsDevice::CreateGPUBuffer(const GPUBufferDesc& _desc) {
	_desc.access;
	_desc.size;
	_desc.usage;
	GLuint handleId{};
	glCreateBuffers(1, &handleId);

	GLenum storageFlags{};
	if (((uint32_t)_desc.access & (uint32_t)BufferCPUAccess::Update) != false) {
		storageFlags |= GL_DYNAMIC_STORAGE_BIT;
	}

	glNamedBufferStorage(handleId, _desc.size, nullptr, storageFlags);
	GL_DYNAMIC_STORAGE_BIT;
	
	GPUBuffer buffer;
	buffer.SetSize(_desc.size);
	

	
	return buffer;
}
