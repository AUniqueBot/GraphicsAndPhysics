#include <arch/systems/sys_render_modules/sys_render_compositor.h>




void CompositorNodeFactory::Init() {
	RegisterNode({ "Render Target", "I/O", {}, { { "Result", COLOR, 0 } } });
	RegisterNode({ "Viewer", "I/O", {{ "Input", COLOR, 0 } }, {} });
}


void CompositorNodeFactory::RegisterNode(CompositionNodeMetadata _def) {
	m_definitions.Add(std::move(_def), _def.m_nodeName);
}
void CompositorNodeFactory::DeregisterNode(std::string _name) {
	m_definitions.Remove(_name);
}
CompositionNode CompositorNodeFactory::CreateNode(std::string _name) {
	CompositionNodeMetadata def	{ *m_definitions.At(_name) };
	CompositionNode node{};

	node.Name(def.m_nodeName);
	for (const auto& input : def.m_inputs) {
		SlotType inSlotData{ input, true };
		node.AddInput(inSlotData);
	}
	for (const auto& output : def.m_outputs) {
		SlotType inSlotData{ output, false };
		node.AddOutput(inSlotData);
	}
	return node;
}

bool CompositorNodeFactory::IsNodeRegistered(std::string _name) const {
	return static_cast<bool>(m_definitions.At(_name));
}

SparseSet<std::string, CompositionNodeMetadata>& CompositorNodeFactory::GetNodeDefinitionsList() {
	return m_definitions;
}
const SparseSet<std::string, CompositionNodeMetadata>& CompositorNodeFactory::GetNodeDefinitionsList() const {
	return m_definitions;
}

// ----------------------------------------------------------------------------------- //
// ----------------------------------------------------------------------------------- //

void Compositor::Init() {
	m_nodeFactory.Init();
}

void Compositor::AddCompositionEffect(const CompositionEffect& _compositionEffect) {

}
void Compositor::RemoveCompositionEffect() {

}


bool Compositor::DoesNodeTypeExist(std::string _name) const {
	return m_nodeFactory.IsNodeRegistered(_name);
}

NodeID Compositor::AddNode(std::string _name) {
	CompositionNode cn{ m_nodeFactory.CreateNode(_name) };
	NodeID id			{ GenerateNodeID() };
	cn.SetID(id);
	m_compositionNodes.Add(std::move(cn), id);
	return id;
}
void Compositor::RemoveNode(NodeID _toRemove) {
	m_compositionNodes.Remove(_toRemove);
	m_compositionLinks.erase(_toRemove);
}



SparseSet<NodeID, CompositionNode>& Compositor::GetNodeList() {
	return m_compositionNodes;
}
const SparseSet<NodeID, CompositionNode>& Compositor::GetNodeList() const {
	return m_compositionNodes;
}

CompositorNodeFactory& Compositor::GetNodeFactory() {
	return m_nodeFactory;
}
const CompositorNodeFactory& Compositor::GetNodeFactory() const {
	return m_nodeFactory;
}

NodeID Compositor::GenerateNodeID() {
	if (m_freeNodeIDList.size()) {
		NodeID id{m_freeNodeIDList.back()};
		return id;
	}
	return ++m_nodeCount;
}
