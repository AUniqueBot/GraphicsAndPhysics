#pragma once
#include <span>
#include <graphics/gfx_renderpass.h>
#include <arch/components/comp_light.h>
#include <arch/components/comp_meshrenderer.h>


class SceneRes; // fwd decl



class ShadowPass {
public:
	void Execute(
		GraphicsContext& _context, 
		const PreparedSceneRenderData& _currentScene
	);

};


static_assert(RenderPass<ShadowPass>);