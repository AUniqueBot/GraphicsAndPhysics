#pragma once

#include <arch/components/componentList.h>
#include <arch/common/entityid.h>
#include <bitset>



class EntityRegistry;

class Entity {

public:
	Entity(EntityRegistry* _registry) : m_registry{ _registry }	{};
	~Entity()													{ m_registry = nullptr; }

	EntityID GetID() const										{ return m_id; };
	bool IsValid() const										{ return m_id.IsValid(); }

	
	template <typename T>
	void AddComponent();

	template <typename T>
	void RemoveComponent() {}


	EntityRegistry* GetRegistry()								{ return m_registry; }
	const EntityRegistry* GetRegistry() const					{ return m_registry; }

private:

	EntityRegistry* m_registry									{};
	EntityID m_id;
	std::bitset<ComponentType::COUNT> m_flags;

};
