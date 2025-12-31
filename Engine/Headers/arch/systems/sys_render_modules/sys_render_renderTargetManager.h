#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>
#include <unordered_map>
class RenderTargetManager {
public:


	RenderTargetManager() = default;
	~RenderTargetManager();

	using RenderTargetID = unsigned;
	RenderTargetID AddRenderTarget(std::string _name, glm::ivec2 _dimensions);
	std::shared_ptr<RenderTarget> GetRenderTarget(RenderTargetID _id);




private:
	std::unordered_map<RenderTargetID, std::shared_ptr<RenderTarget>> m_renderTargetMap;
	static inline unsigned m_renderTargetIdCounter{};
};