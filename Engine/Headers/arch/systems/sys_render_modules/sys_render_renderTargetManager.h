#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>
#include <unordered_map>


class RenderTargetManager {
public:
	using RenderTargetID = unsigned;
	using RenderTargetMap = std::unordered_map<RenderTargetID, std::shared_ptr<RenderTarget>>;
public:


	RenderTargetManager() = default;
	~RenderTargetManager();

	RenderTargetID AddRenderTarget(std::string _name, glm::ivec2 _dimensions);
	std::shared_ptr<RenderTarget> GetRenderTarget(RenderTargetID _id);
	void RemoveRenderTarget(RenderTargetID _rtId);


	const RenderTargetMap& GetRenderTargetMap() const;

private:
	std::unordered_map<RenderTargetID, std::shared_ptr<RenderTarget>> m_renderTargetMap;
	static inline unsigned m_renderTargetIdGenerator			{  };
	static inline std::vector<unsigned> m_freeRenderTargetID	{  };
};