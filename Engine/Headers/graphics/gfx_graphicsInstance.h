#pragma once
#include <pch.h>
#include <graphics/gfx_graphicsContext.h>
#include <graphics/gfx_graphicsDevice.h>


class GraphicsInterface {
public:
	//GraphicsInterface(
	//	GraphicsDevice* _device = nullptr, 
	//	GraphicsContext* _context
	//);


private:
	GraphicsDevice* m_device;
	GraphicsContext* m_context;
};
/*
	what's required
	GraphicsInterface is a bundle containing the 
		- graphics device (gpures factory)
		- graphics context (commands)
*/