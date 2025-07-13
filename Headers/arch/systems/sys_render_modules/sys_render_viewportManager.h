#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>



class ViewportManager {
public:

	void CreateViewport();
private:

	void SortContainerList(); //<! sorts the viewport container list by order.
private:
	std::vector<Viewport> m_viewportContainer;
};