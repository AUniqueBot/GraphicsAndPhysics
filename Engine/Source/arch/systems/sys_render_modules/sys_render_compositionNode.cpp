#include <arch/systems/sys_render_modules/sys_render_compositionNode.h>



void SlotType::Name(const std::string& _name) {
	m_name = _name;
}
const std::string& SlotType::Name() const {
	return m_name;
}

void  SlotType::Type(SCALARTYPE _type) {
	m_type = _type;
}
SCALARTYPE  SlotType::Type() const {
	return m_type;
}

bool  SlotType::IsInput() const {
	return m_isInput;
}

SlotID SlotType::ID() const {
	return m_slotId;
}
SlotMetadata SlotType::GetSlotMetadata() const {
	return {
		m_name,
		m_type,
		m_slotId
	};
}
void  SlotType::AssignID(SlotID _id) {
	m_slotId = _id;
}






CompositionNode::CompositionNode() {
	if (s_nodeIdFreeList.size()) {
		m_id = s_nodeIdFreeList.back();
		s_nodeIdFreeList.pop_back();
	}
	else {
		m_id = ++s_nodeIdGen; 
	}
}

CompositionNode::~CompositionNode() {
	s_nodeIdFreeList.push_back(m_id);
}

void CompositionNode::AddInput(SlotType _node) {
	SlotID id = GenerateSlotID(false);
	_node.AssignID(id);
	m_inputs.Add(std::move(_node), id);
}


void CompositionNode::RemoveInput(SlotID _id) {
	m_inputs.Remove(_id);
	m_inputSlotIdFreeList.push_back(_id);
}

void CompositionNode::AddOutput(SlotType _node) {
	SlotID id = GenerateSlotID(false);
	_node.AssignID(id);
	m_outputs.Add(std::move(_node), id);
}

void CompositionNode::Name(std::string _name) {
	 m_name = _name;
}
const std::string& CompositionNode::Name() const {
	return m_name;
}

SparseSet<SlotID, SlotType>& CompositionNode::GetInputs() {
	return m_inputs;
}

const SparseSet<SlotID, SlotType>& CompositionNode::GetInputs() const {
	return m_inputs;
}

SparseSet<SlotID, SlotType>& CompositionNode::GetOutputs() {
	return m_outputs;
}

const SparseSet<SlotID, SlotType>& CompositionNode::GetOutputs() const {
	return m_outputs;
}



void CompositionNode::RemoveOutput(SlotID _id) {
	m_outputs.Remove(_id);
	m_outputSlotIdFreeList.push_back(_id);
}

void CompositionNode::SetID(NodeID _id) {
	m_id = _id;
}

SlotID CompositionNode::GenerateSlotID(bool _isInput) {
	if (_isInput) {
		if (m_inputSlotIdFreeList.size()) {
			SlotID v = m_inputSlotIdFreeList.back();
			m_inputSlotIdFreeList.pop_back();
			return v;
		}
		return ++m_inputSlotIdGen;
	}
	else {
		if (m_outputSlotIdFreeList.size()) {
			SlotID v = m_outputSlotIdFreeList.back();
			m_outputSlotIdFreeList.pop_back();
			return v;
		}

		return ++m_outputSlotIdGen;
	}
}
