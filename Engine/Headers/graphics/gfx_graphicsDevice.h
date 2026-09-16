#pragma once
#include <pch.h>
#include <graphics/resources/gfx_gpuresource.h>
#include <graphics/device/gfx_gpuresourcemanager.h>

class GraphicsDevice {
	virtual GPUBuffer CreateGPUBuffer(const GPUBufferDesc& _bufferDesc) { return GPUBuffer(); };

private:
	virtual GPUBuffer CreateGPUBufferImpl(const GPUBufferDesc& _bufferDesc) = 0;
};