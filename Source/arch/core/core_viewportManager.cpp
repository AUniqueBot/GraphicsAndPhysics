#pragma once
#include <pch.h>
#include <arch/core/core_viewportManager.h>




void ViewportManager::SortContainerList() {
	std::stable_sort(
		m_viewportContainer.begin(),
		m_viewportContainer.end(),
		[](const Viewport& _a, const Viewport& _b) {
			return _a.ViewportRenderOrder() < _b.ViewportRenderOrder();
		}
	);
}
