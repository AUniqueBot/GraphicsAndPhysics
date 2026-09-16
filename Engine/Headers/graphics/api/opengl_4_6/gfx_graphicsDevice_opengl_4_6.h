#pragma once
#include <pch.h>
#include <graphics/gfx_graphicsDevice.h>
#include <graphics/resources/gfx_resourcelist.h>

class OpenGL4_6_GraphicsDevice : public GraphicsDevice {

	GPUBuffer CreateGPUBuffer(const GPUBufferDesc& _desc) override;

};