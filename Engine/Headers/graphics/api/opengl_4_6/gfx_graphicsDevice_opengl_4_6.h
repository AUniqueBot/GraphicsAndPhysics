#pragma once
#include <pch.h>
#include <graphics/gfx_graphicsDevice.h>
#include <graphics/resources/gfx_resourcelist.h>

class OpenGL4_6_GraphicsDevice : public GraphicsDevice {
public:

private:
	GPUResourceHandle CreateGPUBufferImpl(const GPUBufferDesc& _desc) override;
	GPUResourceHandle CreateGPUTextureImpl(const GPUTextureDesc& _desc) override;


private:
	TrackedStorage<GPUBuffer> m_bufferStorage;
	TrackedStorage<GPUTexture> m_textureStorage;
};