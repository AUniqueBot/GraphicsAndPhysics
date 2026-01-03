#pragma once

#include <bitset>
#include <arch/common/entityid.h>
#include <arch/common/sparseSetViewHandles.h>
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

	bool IsVisible() const										{ return m_visible; }
	void IsVisible(bool _s)										{ m_visible = _s; }


	std::string Name() const									{ return m_name; };
	void Name(std::string _newName)								{ m_name = _newName; };

	
	template <typename T>
	void AddComponent();

	template <typename T>
	void RemoveComponent();


	template <typename T>
	ComponentView<T> GetComponent() const;


	EntityRegistry* GetRegistry()								{ return m_registry; }
	const EntityRegistry* GetRegistry() const					{ return m_registry; }


	const std::set<ComponentMetadata::CompTypeID>& 
		GetAttachedComponents() const							{ return m_componentsAttached; };

protected:
	friend class EntityRegistry;
	std::set<ComponentMetadata::CompTypeID> m_componentsAttached;


private:

	EntityRegistry* m_registry									{};
	EntityID m_id;
	std::string m_name											{};
	std::bitset<ComponentType::COUNT> m_flags;
	bool m_active												{ true };
	bool m_visible												{ true };

};
