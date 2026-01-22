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
constexpr unsigned C_INVALID_NODEID { };
constexpr unsigned C_INVALID_SLOTID { };
struct CompositionLink {
	NodeID m_fromNode;
	SlotID m_fromSlot;

	NodeID m_toNode;
	SlotID m_toSlot;
};


class SlotType {
public:
	SlotType(std::string _name, SCALARTYPE _type, bool _isInput) :
		m_isInput{ _isInput }, m_name{ _name }, m_type{ _type }, m_slotId{C_INVALID_SLOTID}
	{}

	void Name(const std::string& _name);
	const std::string& Name() const;

	void Type(SCALARTYPE _type);
	SCALARTYPE Type() const;

	bool IsInput() const;

	SlotID ID() const;
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
	CompositionNode();
	~CompositionNode();

	void AddInput(SlotType _node); // will assign a slot id (input)
	void RemoveInput(SlotID _id);
	
	void AddOutput(SlotType _node); // will assign a slot id (output, from m_outputSlotIdGen)
	void RemoveOutput(SlotID _id);


private:
	SlotID GenerateSlotID(bool _isInput);
private:
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
};


/*
	bare minimum

	make a DAG

*/