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
using NodeID	= int32_t;
using SlotID	= int32_t;
using PinID		= int32_t;
using LinkID	= int32_t;
static constexpr unsigned C_INVALID_NODEID { };
static constexpr unsigned C_INVALID_SLOTID { };

struct SlotMetadata {
	std::string m_name;
	SCALARTYPE m_type;
	SlotID m_slotID;
};


struct PackedPinInfo {
	NodeID m_node;
	SlotID m_slot;
	bool   m_isInput;

	// ---- bit layout -------------------------------------------------

	static constexpr uint32_t IO_BITS = 1;
	static constexpr uint32_t SLOT_BITS = 14;
	static constexpr uint32_t NODE_BITS = 14;
	static constexpr uint32_t PAD_BITS = 3;

	static_assert(IO_BITS + SLOT_BITS + NODE_BITS + PAD_BITS == 32);

	// ---- shifts -----------------------------------------------------

	static constexpr uint32_t IO_SHIFT = 0;
	static constexpr uint32_t SLOT_SHIFT = IO_SHIFT + IO_BITS;    // 1
	static constexpr uint32_t NODE_SHIFT = SLOT_SHIFT + SLOT_BITS;  // 15
	static constexpr uint32_t PAD_SHIFT = NODE_SHIFT + NODE_BITS;  // 29

	// ---- masks ------------------------------------------------------

	static constexpr uint32_t IO_MASK = (1u << IO_BITS) - 1;     // 0x1
	static constexpr uint32_t SLOT_MASK = (1u << SLOT_BITS) - 1;     // 0x3FFF
	static constexpr uint32_t NODE_MASK = (1u << NODE_BITS) - 1;     // 0x3FFF

	// ---- packing ----------------------------------------------------

	static PinID ComposePinID(NodeID nodeId, SlotID slotId, bool isInput)
	{
		return
			(static_cast<PinID>(nodeId & NODE_MASK) << NODE_SHIFT) |
			(static_cast<PinID>(slotId & SLOT_MASK) << SLOT_SHIFT) |
			static_cast<PinID>(isInput);
	}

	// ---- unpacking --------------------------------------------------

	static PackedPinInfo DecomposePinID(PinID id)
	{
		PackedPinInfo info{};
		info.m_isInput = (id >> IO_SHIFT) & IO_MASK;
		info.m_slot = (id >> SLOT_SHIFT) & SLOT_MASK;
		info.m_node = (id >> NODE_SHIFT) & NODE_MASK;
		return info;
	}
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
	PinID m_fromPin		{};
	PinID m_toPin		{};

	NodeID m_fromNode	{};
	NodeID m_toNode		{};


	void SetPins(PinID _from, PinID _to) {
		PackedPinInfo fromPacked{ PackedPinInfo::DecomposePinID(_from) };
		PackedPinInfo toPacked{ PackedPinInfo::DecomposePinID(_to) };

		m_fromPin = _from;
		m_toPin = _to;
		m_fromNode = fromPacked.m_node;
		m_toNode = toPacked.m_node;
	}
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

	PinID GetPinID() const;

protected:
	friend class CompositionNode;
	void GenerateIDs(NodeID _nodeId, SlotID _slotId, bool _isInput);
	void AssignSlotID(SlotID _id);
	void AssignPinID(NodeID _nodeId, bool _isInput);

private:
	SCALARTYPE m_type; // for later
	SlotID m_slotId;
	PinID m_pinId; // unique identifier for pins
	std::string m_name;
	bool m_isInput;
};


class CompositionNode {
public:
	friend class Compositor;
public:
	CompositionNode();
	~CompositionNode();

	void AddInput(SlotType _slot); // will assign a slot id (input)
	void RemoveInput(SlotID _id);
	
	void AddOutput(SlotType _slot); // will assign a slot id (output, from m_outputSlotIdGen)
	void RemoveOutput(SlotID _id);

	void Name(std::string _name);
	const std::string& Name() const; 

	NodeID GetID() const { return m_id; }

	SparseSet<SlotID, SlotType>& GetInputs();
	const SparseSet<SlotID, SlotType>& GetInputs() const;
	// save data.
	SparseSet<SlotID, SlotType>& GetOutputs();
	const SparseSet<SlotID, SlotType>& GetOutputs() const;

	glm::vec2& Position() { return m_position; }


	void AddLink(LinkID _linkId, bool _asInput);
	void RemoveLink(LinkID _linkId);
	void RemoveLink(LinkID _linkId, bool _isInput);





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


	std::vector<LinkID> m_inputLinks;
	std::vector<LinkID> m_outputLinks;


	glm::vec2 m_position								{}; // for ImGui.
};





/*
	bare minimum

	make a DAG

*/
