#pragma once
#include <pch.h>
#include <graphics/gfx_graphicsContext.h>


class GraphicsContext_OpenGL4_6 : public GraphicsContext {



protected:
	void DrawImpl(const GraphicsContextTypes::DrawArgs& _args) override;
	void DrawIndexedImpl(const GraphicsContextTypes::DrawIndexedArgs& _args) override;


};