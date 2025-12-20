#include <arch/systems/sys_render_modules/sys_render_renderTargetManager.h>

RenderTargetManager::~RenderTargetManager() {
	for (auto& [_, rt] : m_renderTargetMap) {
		rt.Destroy();
	}
}

unsigned RenderTargetManager::AddRenderTarget(std::string _name, glm::ivec2 _dimensions) {
	const unsigned id = m_renderTargetIdCounter++;
	RenderTarget renderTarget{};
	renderTarget.Create(_dimensions);
	m_renderTargetMap.emplace(id, std::move(renderTarget));
	return id;
}
