#pragma once
#include <vector>
#include <forward_list>
#include <arch/systems/sys_render_modules/sys_render_renderLayer.h>

class RenderManager {
	// contains stuff idk.

public:
	
	void AddRenderLayer();

private:
	// you have multiple paths for each render
	// final step is always a composited version of this setup
	// admittedly a linked list makes the most sense in this context
	std::vector<std::vector<RenderLayer>> m_compositorSetup;
};