#pragma once
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>
#include <unordered_map>
class RenderTargetManager {
public:
	RenderTargetManager() = default;
	~RenderTargetManager();


	unsigned AddRenderTarget(std::string _name, glm::ivec2 _dimensions);





private:
	std::unordered_map<unsigned, RenderTarget> m_renderTargetMap;
	static inline unsigned m_renderTargetIdCounter{};
};