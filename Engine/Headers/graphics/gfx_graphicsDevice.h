#pragma once
#include <pch.h>
#include <graphics/resources/gfx_gpuresource.h>
#include <graphics/resources/gfx_gpuhandle.h>
#include <graphics/device/gfx_gpuresourcemanager.h>

class GraphicsDevice {
	inline GPUResourceHandle CreateGPUBuffer(const GPUBufferDesc& _bufferDesc) { 
		return CreateGPUBufferImpl(_bufferDesc); 
	};

	inline GPUResourceHandle CreateGPUTexture(const GPUTextureDesc& _desc) {
		return CreateGPUTextureImpl(_desc);
	}

private:
	virtual GPUResourceHandle CreateGPUBufferImpl(const GPUBufferDesc& _bufferDesc) = 0;
	virtual GPUResourceHandle CreateGPUTextureImpl(const GPUTextureDesc& _texDesc) = 0;
};