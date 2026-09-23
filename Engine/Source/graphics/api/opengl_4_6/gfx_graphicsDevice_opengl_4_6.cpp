#include <graphics/api/opengl_4_6/gfx_graphicsDevice_opengl_4_6.h>

GPUResourceHandle OpenGL4_6_GraphicsDevice::CreateGPUBufferImpl(
	const GPUBufferDesc& _desc
) {
	GPUBuffer buffer;
	GPURES_ID id { m_bufferStorage.AddResource(std::move(buffer)) };
	
	
	return GPUResourceHandle {
		.m_id = id,
		.m_type = GPUDatatype::Buffer
	};

}

GPUResourceHandle OpenGL4_6_GraphicsDevice::CreateGPUTextureImpl(
	const GPUTextureDesc& _desc
) {
	GPUTexture tex(_desc.textureType, _desc.dimensions);
	tex.Create();
	tex.Allocate();
	GPURES_ID id = m_textureStorage.AddResource(std::move(tex));
	return GPUResourceHandle{
		.m_id = 0, 
		.m_type = GPUDatatype::Texture
	};

}
