#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_viewportManager.h>
#include <arch/systems/sys_render_modules/sys_render_renderTargetManager.h>



Viewport::ViewportID ViewportManager::CreateViewportInternal() {
	Viewport::ViewportID id	{ m_nextId };
	std::shared_ptr<Viewport> newVP			{ std::make_shared<Viewport>(id) };
	++m_nextId;

	newVP->Init();
	m_viewportIndexRenderOrder.push_back(id);
	m_viewports.emplace( id, newVP);

	return id;
}


ViewportManager::ViewportManager(RenderTargetManager& _rtm) : m_renderTargetManager{ _rtm }
{
}

Viewport::ViewportID ViewportManager::CreateViewport() {
	RenderTargetManager& rtm = m_renderTargetManager;

	Viewport::ViewportID vpId	{ CreateViewportInternal() };
	Viewport& viewport			{ *GetViewport(vpId) };
	std::string viewportName = "ViewportRT" + vpId;


	RenderTargetManager::RenderTargetID rtId1 { 
		rtm.AddRenderTarget(
			viewport.ViewportDimensions(),
			viewportName + "_p1"
		) 
	};

	RenderTargetManager::RenderTargetID rtId2 {
	rtm.AddRenderTarget(
		viewport.ViewportDimensions(),
		viewportName + "_p2"
	)
	};


	RenderTarget& renderTarget1			{ *rtm.GetRenderTarget(rtId1) };
	std::shared_ptr<RenderTarget> rt1	{ std::make_shared<RenderTarget>(renderTarget1) };
	rt1->AddInitialColorAttachments();
	rt1->Build();

	RenderTarget& renderTarget2{ *rtm.GetRenderTarget(rtId2) };
	std::shared_ptr<RenderTarget> rt2{ std::make_shared<RenderTarget>(renderTarget2) };
	rt2->AddInitialColorAttachments();
	rt2->Build();

	viewport.SetPingPongRT(rt1, rt2);
	return vpId;
}



std::shared_ptr<Viewport> ViewportManager::GetViewport(Viewport::ViewportID _id) {
	return 
		m_viewports.find(_id) == m_viewports.end() ?
		nullptr : m_viewports.at(_id);
}

void ViewportManager::RemoveViewport(Viewport::ViewportID _id) {
	if (!m_viewports.contains(_id)) return;
	m_viewports.erase(_id);
	m_viewportIndexRenderOrder.erase(
		std::remove(m_viewportIndexRenderOrder.begin(), m_viewportIndexRenderOrder.end(), _id),
		m_viewportIndexRenderOrder.end()
	);
}

std::unordered_map<Viewport::ViewportID, std::shared_ptr<Viewport>>& 
	ViewportManager::ViewportList() {
	return m_viewports;
}

const std::unordered_map<Viewport::ViewportID, std::shared_ptr<Viewport>>& 
	ViewportManager::ViewportList() const {
	return m_viewports;
}

void ViewportManager::RequestViewportSort() {
	m_orderDirty = true;
}

const std::vector<Viewport::ViewportID>& ViewportManager::ViewportRenderOrderList() const {
	return m_viewportIndexRenderOrder;
}



void ViewportManager::SortRenderOrder() {
	std::stable_sort(
		m_viewportIndexRenderOrder.begin(),
		m_viewportIndexRenderOrder.end(),
		[this](uint32_t a, uint32_t b) {
			return m_viewports.at(a)->ViewportRenderOrder() < m_viewports.at(b)->ViewportRenderOrder();
		}
	);
}
