#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_compositionEffect.h>
#include <arch/systems/sys_render_modules/sys_render_compositionNode.h>
#include <arch/datatypes/type_sparseSet.h>


// this factory does not care about what has it or something.
class CompositorNodeFactory {
public:

	void Init();
	void RegisterNode(CompositionNodeMetadata _def);
	void DeregisterNode(std::string _name);
	bool IsNodeRegistered(std::string _name) const;
	CompositionNode CreateNode(std::string _name);


	SparseSet<std::string, CompositionNodeMetadata>& GetNodeDefinitionsList();
	const SparseSet<std::string, CompositionNodeMetadata>& GetNodeDefinitionsList() const;

private:
	SparseSet<std::string, CompositionNodeMetadata> m_definitions;
};



class Compositor {


public:

	void Init();
	// NOTE: This only adds the composition to the pool of stuff you are registering.
	void AddCompositionEffect(const CompositionEffect& _compositionEffect);
	void RemoveCompositionEffect();

public:

	bool DoesNodeTypeExist(std::string _name) const;
	NodeID AddNode(std::string _name);
	void RemoveNode(NodeID _toRemove);


	SparseSet<NodeID, CompositionNode>& GetNodeList();
	const SparseSet<NodeID, CompositionNode>& GetNodeList() const;

	void AddLink(NodeID _fromNode, SlotID _fromSlot, NodeID _toNode, SlotID _toSlot);
	void RemoveLink();


	CompositorNodeFactory& GetNodeFactory();
	const CompositorNodeFactory& GetNodeFactory() const;


private:
	NodeID GenerateNodeID();

private:
	void* m_inputTexture;

	
	std::vector<std::unique_ptr<CompositionEffect>> m_compositionEffectsList;
	SparseSet<NodeID, CompositionNode> m_compositionNodes;

	std::unordered_map<NodeID, CompositionLink> m_compositionLinks; // group by source.
	
	NodeID m_nodeCount						{};
	std::vector<NodeID> m_freeNodeIDList	{};

	CompositorNodeFactory m_nodeFactory;

	void* m_outputTexture;
};



