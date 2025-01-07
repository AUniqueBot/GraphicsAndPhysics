#pragma once
#include <pch.h>


#include <unordered_map>
#include <arch/datatypes/type_sparseset.h>
#include <arch/common/entity.h>

#include <arch/components/componentList.h>
#include <arch/common/component.h>

#include <typeindex>



// component pool class
// ------------------------------------------------------------------------------------
// base class to utilise polymorphism
struct IComponentPool {
	~IComponentPool() = default;

};


template <typename T>
class ComponentPool : public IComponentPool {

	// data struct containing all components here.
	SparseSet<EntityID, T> m_compPool;

public:

	///! @brief get the data of the component pool.
	///! @return 
	SparseSet<EntityID, T>& Data();
	const SparseSet<EntityID, T>& Data() const;


	bool Add(EntityID _addTo);
	bool Remove(EntityID _removeFrom);
};

// ------------------------------------------------------------------------------------
class EntityRegistry {


public:

	EntityRegistry() = default;

	// component registration.
	template <typename T>
	void RegisterType();

	template <typename T>
	std::optional<ComponentPool<T>&> GetComponentPool();

	template <typename T>
	const std::optional<ComponentPool<T>&> GetComponentPool() const;


	
	// component handling.

	///! @brief adds specified component to the entity given.
	///! @tparam T: Component Type
	///! @param _addTo: entity id to add a component to
	///! @return true if successful.
	template <typename T>
	bool AddComponent(EntityID _addTo);


	///! @brief removes specified component from the entity given.
	///! @tparam T: Component Type
	///! @param _removeFrom: entity id to remove component from 
	///! @return true if successful.
	template <typename T>
	bool RemoveComponent(EntityID _removeFrom);

private:

	using CompType = std::type_index;

	std::vector<EntityID> m_entityList;
	
	std::unordered_map<CompType, std::shared_ptr<IComponentPool>> m_componentPool;


};





#include <arch/ecs/ecs_registryDEF.hpp>