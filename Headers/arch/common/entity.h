#pragma once

#include <bitset>
#include <arch/common/entityid.h>
#include <arch/common/componentView.h>
#include <arch/components/componentList.h>



class EntityRegistry;

class Entity {

public:
	Entity(EntityRegistry* _registry) : m_registry{ _registry }	{};
	~Entity()													{ m_registry = nullptr; }

	EntityID GetID() const										{ return m_id; };
	bool IsValid() const										{ return m_id.IsValid(); }

	bool Active() const											{ return m_active; }
	void Active(bool _s)										{ m_active = _s; }

	
	template <typename T>
	void AddComponent();

	template <typename T>
	void RemoveComponent();


	template <typename T>
	ComponentView<T> GetComponent() const;


	EntityRegistry* GetRegistry()								{ return m_registry; }
	const EntityRegistry* GetRegistry() const					{ return m_registry; }

private:

	EntityRegistry* m_registry									{};
	EntityID m_id;
	std::bitset<ComponentType::COUNT> m_flags;
	bool m_active												{ true };

};
