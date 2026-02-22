#include <arch/systems/sys_render_modules/sys_render_renderTargetManager.h>

RenderTargetManager::~RenderTargetManager() {
	for (auto& [_, rt] : m_renderTargetMap) {
		if (rt) rt->Destroy();
	}
}

RenderTargetManager::RenderTargetID RenderTargetManager::AddRenderTarget(std::string _name, glm::ivec2 _dimensions) {
	bool existingFreeID	{ m_freeRenderTargetID.size() > 0 };
	const unsigned id = existingFreeID  ?
		m_freeRenderTargetID.back() : m_renderTargetIdGenerator++;
	if (existingFreeID) m_freeRenderTargetID.pop_back();
	std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>();
	renderTarget->Resolution(_dimensions);
	//renderTarget->Create_DEC(_dimensions); // to be deprecated.
	m_renderTargetMap.emplace(id, renderTarget);
	return id;
}

std::shared_ptr<RenderTarget> RenderTargetManager::GetRenderTarget(RenderTargetID _id) {
	// TODO: insert return statement here
	return m_renderTargetMap.at(_id);
}

void RenderTargetManager::RemoveRenderTarget(RenderTargetID _rtId) {
	
	m_renderTargetMap[_rtId]->Destroy();
	m_renderTargetMap.erase(_rtId);
	m_freeRenderTargetID.push_back(_rtId);
}

const RenderTargetManager::RenderTargetMap& RenderTargetManager::GetRenderTargetMap() const {
	return m_renderTargetMap;
}
