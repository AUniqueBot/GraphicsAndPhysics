#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>



class ViewportManager {

public:
	Viewport::ViewportID CreateViewport();
	void RemoveViewport(Viewport::ViewportID _id);

	std::map<Viewport::ViewportID, Viewport>& ViewportList();
	const std::map<Viewport::ViewportID, Viewport>& ViewportList() const;


	void RequestViewportSort();
	
	const std::vector<Viewport::ViewportID>& ViewportRenderOrderList() const;

private:

	void SortRenderOrder();

private:
	bool m_orderDirty								{ true };

	glm::vec2 m_sensitivity							{ 1, 1 };

	Viewport::ViewportID m_nextId					{ 1 };
	std::map<Viewport::ViewportID, Viewport> m_viewports;
	std::vector<Viewport::ViewportID> m_viewportIndexRenderOrder;


};