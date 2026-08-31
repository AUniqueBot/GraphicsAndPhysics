#pragma once
#include <arch/common/entityid.h>
#include <arch/resources/res_resource.h>

struct UI_Selectable {
	enum Type {
		GameObject,
		Resource,
		NONE
	};

	UI_Selectable() = default;
	UI_Selectable& operator=(const UI_Selectable&) = default;

	explicit UI_Selectable(const EntityID& _id) : m_type{ 
		_id != EntityConstants::C_ENTITYID_INVALID ? GameObject : NONE 
	}, m_id{ static_cast<EntityIDType>(_id) } {}

	explicit UI_Selectable(const RES_ID& _id) : m_type{ 
		_id != ResourceConstants::C_RES_INVALID_ID ? Resource : NONE
	}, m_id{ _id } {}

	union ID {
		RES_ID m_resId;
		EntityIDType m_entityId;
	} m_id{ EntityConstants::C_ENTITYID_INVALID };
	Type m_type{ NONE };

	bool operator==(const UI_Selectable& _other) const;
};
