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
PinID SlotType::GetPinID() const {
	return m_pinId;
}
void SlotType::GenerateIDs(NodeID _nodeId, SlotID _slotId, bool _isInput) {
	AssignSlotID(_slotId);
	AssignPinID(_nodeId, _isInput);
}
void SlotType::AssignSlotID(SlotID _id) {
	m_slotId = _id;
}

void SlotType::AssignPinID(NodeID _nodeId, bool _isInput){
	// 16 bits node, 15 bits pinIndex, 1 bit type
	m_pinId = PackedPinInfo::ComposePinID(_nodeId, m_slotId, _isInput); 
}



CompositionNode::CompositionNode() {
	//if (s_nodeIdFreeList.size()) {
	//	m_id = s_nodeIdFreeList.back();
	//	s_nodeIdFreeList.pop_back();
	//}
	//else {
	//	++s_nodeIdGen;
	//	m_id = s_nodeIdGen;
	//}
}

CompositionNode::~CompositionNode() {
	//s_nodeIdFreeList.push_back(m_id);
}

void CompositionNode::AddInput(SlotType _inputSlot) {
	SlotID id = GenerateSlotID(false);
	_inputSlot.GenerateIDs(m_id, id, true);
	m_inputs.Add(std::move(_inputSlot), id);
}


void CompositionNode::RemoveInput(SlotID _id) {
	m_inputs.Remove(_id);
	m_inputSlotIdFreeList.push_back(_id);
}

void CompositionNode::AddOutput(SlotType _inputSlot) {
	SlotID id = GenerateSlotID(false);
	_inputSlot.GenerateIDs(m_id, id, false);
	m_outputs.Add(std::move(_inputSlot), id);
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
	if (_id == m_id) return;
	m_id = _id;
	// regen ids;
	for (auto& input : m_inputs.Data()) {
		input.GenerateIDs(m_id, input.m_slotId, true);
	}
	for (auto& output : m_outputs.Data()) {
		output.GenerateIDs(m_id, output.m_slotId, true);
	}

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
