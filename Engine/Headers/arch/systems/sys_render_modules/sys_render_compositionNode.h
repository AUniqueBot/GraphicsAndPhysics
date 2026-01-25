#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_compositionEffect.h>
#include <arch/datatypes/type_sparseSet.h>
class CompositionNode;

enum SCALARTYPE {
	INT,			//
	UINT,			//
	FLOAT,			//
	VECTOR,			//
	COLOR,			// vec4
};
using NodeID = unsigned;
using SlotID = unsigned;
using LinkID = unsigned;
static constexpr unsigned C_INVALID_NODEID { };
static constexpr unsigned C_INVALID_SLOTID { };

struct SlotMetadata {
	std::string m_name;
	SCALARTYPE m_type;
	SlotID m_slotID;
};


struct CompositionLink {

	CompositionLink() {
		if (m_freeLinkIDList.size()) {
			m_linkId = m_freeLinkIDList.back();
			m_freeLinkIDList.pop_back();
		}
		else {
			m_linkId = ++m_linkCounter;
		}
	}
	NodeID m_fromNode	{};
	SlotID m_fromSlot	{};

	NodeID m_toNode		{};
	SlotID m_toSlot		{};

	LinkID GetLinkID() const { return m_linkId; }

private:
	LinkID m_linkId{};
	inline static LinkID m_linkCounter					{};
	inline static std::vector<LinkID> m_freeLinkIDList	{};
};

struct CompositionNodeMetadata {
	std::string m_nodeName;
	std::string m_category; // unused for now.

	// function pointer here.


	std::vector<SlotMetadata> m_inputs;
	std::vector<SlotMetadata> m_outputs;
};



class SlotType {
public:
	SlotType(std::string _name, SCALARTYPE _type, bool _isInput) :
		m_isInput{ _isInput }, m_name{ _name }, m_type{ _type }, m_slotId{C_INVALID_SLOTID}
	{}
	SlotType(SlotMetadata _slotMetadata, bool _isInput) :
		m_isInput{ _isInput }, m_name{ _slotMetadata.m_name }, m_type{ _slotMetadata.m_type }, m_slotId{ C_INVALID_SLOTID }
	{

	}
	void Name(const std::string& _name);
	const std::string& Name() const;

	void Type(SCALARTYPE _type);
	SCALARTYPE Type() const;

	bool IsInput() const;

	SlotID ID() const;

	SlotMetadata GetSlotMetadata() const;

protected:
	friend class CompositionNode;
	void AssignID(SlotID _id);

private:
	SCALARTYPE m_type; // for later
	SlotID m_slotId;
	std::string m_name;
	bool m_isInput;
};


class CompositionNode {
public:
	friend class Compositor;
public:
	CompositionNode();
	~CompositionNode();

	void AddInput(SlotType _node); // will assign a slot id (input)
	void RemoveInput(SlotID _id);
	
	void AddOutput(SlotType _node); // will assign a slot id (output, from m_outputSlotIdGen)
	void RemoveOutput(SlotID _id);

	void Name(std::string _name);
	const std::string& Name() const; 

	NodeID GetID() const { return m_id; }

	SparseSet<SlotID, SlotType>& GetInputs();
	const SparseSet<SlotID, SlotType>& GetInputs() const;
	// save data.
	SparseSet<SlotID, SlotType>& GetOutputs();
	const SparseSet<SlotID, SlotType>& GetOutputs() const;

	static unsigned GetUniquePinID(int _nodeID, int _localPinID, bool _input) {
		// 16 bits node, 15 bits pinIndex, 1 bit type
		return (_nodeID << 16) | (_localPinID << 1) | static_cast<uint32_t>(_input);
	}

private:
	void SetID(NodeID _id);
	SlotID GenerateSlotID(bool _isInput);
private:
	std::string m_name									{};
	std::string m_nodeName								{};
	inline static NodeID s_nodeIdGen					{};
	inline static std::vector<NodeID> s_nodeIdFreeList	{};
	NodeID m_id;

	SlotID m_inputSlotIdGen								{};
	std::vector<SlotID> m_inputSlotIdFreeList			{};
	SlotID m_outputSlotIdGen							{};
	std::vector<SlotID> m_outputSlotIdFreeList			{};

	void* m_compositionEffect							{};


	SparseSet<SlotID, SlotType> m_inputs;
	SparseSet<SlotID, SlotType> m_outputs;

	glm::vec2 m_position								{}; // for ImGui.
};





/*
	bare minimum

	make a DAG

*/
