#include <arch/systems/sys_render_modules/sys_render_renderTargetManager.h>

RenderTargetManager::~RenderTargetManager() {
	for (auto& [_, rt] : m_renderTargetMap) {
		if (rt) rt->Destroy();
	}
}

RenderTargetManager::RenderTargetID RenderTargetManager::AddRenderTarget(std::string _name, glm::ivec2 _dimensions) {
	const unsigned id = m_renderTargetIdCounter++;
	RenderTarget renderTarget{};
	renderTarget.Create(_dimensions);
	m_renderTargetMap.emplace(id, std::make_shared<RenderTarget>(std::move(renderTarget)));
	return id;
}

std::shared_ptr<RenderTarget> RenderTargetManager::GetRenderTarget(RenderTargetID _id) {
	// TODO: insert return statement here
	return m_renderTargetMap.at(_id);
}
