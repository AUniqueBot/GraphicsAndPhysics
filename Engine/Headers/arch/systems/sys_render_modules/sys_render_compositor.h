#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_compositionEffect.h>
#include <arch/systems/sys_render_modules/sys_render_compositionNode.h>

class Compositor {


public:
	// NOTE: This only adds the composition to the pool of stuff you are registering.
	void AddCompositionEffect(const CompositionEffect& _compositionEffect);
	void RemoveCompositionEffect();



	void AddNode();
	void RemoveNode();

private:
	void* m_inputTexture;

	
	std::vector<std::unique_ptr<CompositionEffect>> m_compositionNodeList;
	std::vector<CompositionNode> m_compositionNodes;
	std::unordered_map<NodeID, CompositionLink> m_compositionLinks; // group by source.
	void* m_outputTexture;
};

