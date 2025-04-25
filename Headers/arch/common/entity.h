#pragma once
// #include <arch/ecs/ecs_registry.h>
#include <bitset>
#include <arch/common/entityid.h>
#include <arch/components/componentList.h>


class EntityRegistry; // fwd declaration.
class Entity {

public:
	Entity(EntityRegistry* _registry) : m_registry{ _registry }	{};
	~Entity()													{ m_registry = nullptr; };

	EntityID GetID() const										{ return m_id; };
	bool IsValid() const										{ return m_id.IsValid(); };

	
	template <typename ComponentType>
	void AddComponent() {
		if (!m_registry) return;
		m_registry->AddComponent<ComponentType>(m_id);
	}

	template <typename Component>
	void RemoveComponent() {

	}


	EntityRegistry* GetRegistry()								{ return m_registry; }
	const EntityRegistry* GetRegistry() const					{ return m_registry; }

private:

	EntityRegistry* m_registry									{};
	EntityID m_id												{};
	std::bitset<ComponentType::COUNT> m_flags;

};

