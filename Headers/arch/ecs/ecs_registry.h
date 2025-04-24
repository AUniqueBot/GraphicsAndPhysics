
#pragma once
#include <pch.h>

#include <arch/datatypes/type_sparseset.h>
#include <arch/common/entity.h>

#include <arch/components/componentList.h>
#include <arch/common/component.h>

#include <entt/entt.hpp>

#include <typeindex>

#define TEMPLATE_BASECOMPONENT template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true> 

// component pool class
// ------------------------------------------------------------------------------------
// base class to utilise polymorphism
class IComponentPool {
public:
	~IComponentPool() = default;
	virtual size_t size() const { return 3; }
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

	size_t size() const override { return m_compPool.size(); };
};

// ------------------------------------------------------------------------------------
class EntityRegistry {


public:

	EntityRegistry() = default;

	
	///! @brief registers a component for use.
	///! @tparam T: Component
	///! @note since the registration of components happens here and it filters out non-components, only this function has a component check
	TEMPLATE_BASECOMPONENT
	void RegisterType() {
		// create a sparse set of type T
		CompType currentComponent{ typeid(T) };
		m_componentPool[currentComponent] = std::make_shared<ComponentPool<T>>();
		m_compRegisterFunctions[currentComponent] = T::Register;


		std::cout << __FUNCSIG__ << ": registering component: " << currentComponent.name() << std::endl;

	}

	void RegisterComponentsForSerialisation() {
		for (auto& [UNUSED, func] : m_compRegisterFunctions)
			func();
	}

	void PrintDebugInfo() const;

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
	std::unordered_map<CompType, std::function<void()>> m_compRegisterFunctions;
	std::unordered_map<CompType, std::shared_ptr<IComponentPool>> m_componentPool;


};




#include <arch/ecs/ecs_registryDEF.hpp>