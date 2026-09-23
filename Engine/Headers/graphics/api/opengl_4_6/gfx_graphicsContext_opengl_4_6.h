#pragma once
#include <pch.h>
#include <graphics/gfx_graphicsContext.h>


class OpenGL4_6_GraphicsContext : public GraphicsContext {


private:

	inline void UpdateBufferImpl(GPUBuffer& _toFill, void* _data, size_t _size) override {};
	inline void DrawImpl(const GraphicsContextTypes::DrawArgs& _args) override {};
	inline void DrawIndexedImpl(const GraphicsContextTypes::DrawIndexedArgs& _args) override {};
};

