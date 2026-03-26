#pragma once
#include <pch.h>
#include <arch/common/entityid.h>
#include <arch/components/componentList.h>
#include <arch/common/component_properties.h>


class Component {
	
public:
	template <std::derived_from<Component> T>
	friend class ComponentPool;

	virtual void Init() { LOG_INFO("Initialised Component"); };
	virtual void End()	{};

	// not intended to be set outside.
	void SetComponentID(ComponentType _type)	{ m_type = _type; }
	ComponentType GetComponentID() const		{ return m_type; }
	
	EntityID GetEntityID() const				{ return m_registeredEntity; } 
	
	// serialization function
	static void Register() { LOG_INFO("Registering Component - Generic. If you see this, you didn't override this in your component."); };

	virtual std::vector<PropertyMD::Property>& GetComponentProperties();
	

	void SetEntityID(const EntityID& _id)		{ m_registeredEntity = _id; };
private:
	ComponentType m_type						{ __INVALID };
	inline static unsigned s_compIdCounter		{ 0 }; // 0 is invalid.
	EntityID m_registeredEntity					{ EntityID::ENTITYID_INVALID };
};

template <typename T>
concept TemplateComponentType = std::derived_from<T, Component>;